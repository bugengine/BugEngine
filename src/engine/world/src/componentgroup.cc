/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <componentgroup.hh>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>
#include    <minitl/algorithm.hh>
#include    <entityinfo.hh>

namespace BugEngine { namespace World
{

namespace
{

static const u32 s_invalidEntity = ~(u32)0;

static raw<const RTTI::Method::Overload> getMethodFromClass(raw<const RTTI::Class> type,
                                                            istring name)
{
    raw<const RTTI::Method> method = type->getMethod(name);
    be_assert_recover(method,
                      "could not locate method \"%s\" for component %s" | name | type->fullname(),
                      return raw<const RTTI::Method::Overload>());
    for (const RTTI::Method::Overload* overload = method->overloads->begin();
         overload != method->overloads->end();
         ++overload)
    {
        RTTI::Type componentType = RTTI::Type::makeType(type, RTTI::Type::Value,
                                                        RTTI::Type::Mutable, RTTI::Type::Mutable);
        if (overload->parameterCount() == 2
         && componentType.isA(overload->params[0].type)
         && be_typeid<World&>::type().isA(overload->params->next->type))
        {
            return overload;
        }
    }
    be_error("%s::%s() overloaded: will not be called by entity manager" | type->fullname() | name);
    return raw<const RTTI::Method::Overload>();
}

}

struct ComponentGroup::EntityOperation
{
    u32 owner;
    u32 maskBefore;
    u32 maskAfter;
    u32 size;

    EntityOperation* next()
    {
        byte* buffer = reinterpret_cast<byte*>(this);
        return reinterpret_cast<EntityOperation*>(buffer + size);
    }
    byte* components()
    {
        return reinterpret_cast<byte*>(this) + sizeof(EntityOperation);
    }
};

struct ComponentGroup::EntityOperationRemove
{
    EntityOperation operation;
    u32             componentIndex;
};

struct ComponentGroup::OperationBuffer
{
    iptr<OperationBuffer>   m_next;
    OperationBuffer*        m_prev;
    i_u32                   m_offset;
    i_u32                   m_used;
    byte                    m_data[1];
    OperationBuffer(OperationBuffer* previous)
        :   m_next(0)
        ,   m_prev(previous)
        ,   m_offset(i_u32::Zero)
        ,   m_used(i_u32::Zero)
        ,   m_data()
    {
    }

    EntityOperation* begin()    { return reinterpret_cast<EntityOperation*>(m_data); }
    EntityOperation* end()      { return reinterpret_cast<EntityOperation*>(m_data + m_used); }

    template< typename T >
    void forEach(EntityOperation* first, T& transform)
    {
        for (EntityOperation* last = end(); first != last; first=first->next())
        {
            transform(this, first);
        }
        OperationBuffer* b = this;
        for (OperationBuffer* next = b->m_next; next; next=next->m_next)
        {
            transform(b, next);
            b = next;
            for (EntityOperation* op = next->begin(), *last = next->end(); op != last; op = op->next())
            {
                transform(next, op);
            }
        }
        transform(b, (OperationBuffer*)0);
    }
    template< typename T >
    void forEach(T& transform)
    {
        return this->forEach(begin(), transform);
    }
};

struct ComponentGroup::EntityOperationRemoveIterator
{
    typedef minitl::random_access_iterator_tag iterator_category;
    typedef EntityOperationRemove value_type;
    typedef EntityOperationRemove* pointer;
    typedef EntityOperationRemove& reference;
    typedef u32 size_type;
    typedef i32 difference_type;

    OperationBuffer*    page;
    u32                 offset;

    EntityOperationRemove* operator->() const
    {
        byte* data = page->m_data + offset;
        return reinterpret_cast<EntityOperationRemove*>(data);
    }

    EntityOperationRemove& operator*() const
    {
        byte* data = page->m_data + offset;
        return *reinterpret_cast<EntityOperationRemove*>(data);
    }

    EntityOperationRemoveIterator& operator++()
    {
        offset += sizeof(EntityOperationRemove);
        if (offset >= page->m_used)
        {
            page = page->m_next;
            offset = 0;
        }
        return *this;
    }

    EntityOperationRemoveIterator operator++(int)
    {
        EntityOperationRemoveIterator result = *this;
        return ++result;
    }

    EntityOperationRemoveIterator& operator--()
    {
        if (offset < sizeof(EntityOperationRemove))
        {
            page = page->m_prev;
            offset = page->m_used - sizeof(EntityOperationRemove);
        }
        else
        {
            offset -= sizeof(EntityOperationRemove);
        }
        return *this;
    }

    EntityOperationRemoveIterator operator--(int)
    {
        EntityOperationRemoveIterator result = *this;
        return --result;
    }

    EntityOperationRemoveIterator operator+(minitl::difference_type delta) const
    {
        EntityOperationRemoveIterator result = { page, offset };
        minitl::difference_type byteOffset = delta * sizeof(EntityOperationRemove);
        while (result.offset + byteOffset > result.page->m_used)
        {
            byteOffset -= (result.page->m_used - result.offset);
            result.page = result.page->m_next;
            result.offset = 0;
        }
        return result;
    }

    EntityOperationRemoveIterator operator-(minitl::difference_type delta) const
    {
        EntityOperationRemoveIterator result = { page, offset };
        minitl::difference_type byteOffset = delta * sizeof(EntityOperationRemove);
        while (byteOffset > (minitl::difference_type)result.offset)
        {
            byteOffset -= result.offset;
            result.page = result.page->m_prev;
            result.offset = result.page->m_offset - sizeof(EntityOperationRemove);
        }
        return result;
    }

    i32 operator-(const EntityOperationRemoveIterator& rhs) const
    {
        if (rhs.page != page)
        {
            be_assert(offset % sizeof(EntityOperationRemove) == 0,
                      "invalid iterator difference");
            u32 diff = offset / sizeof(EntityOperationRemove);
            OperationBuffer* b = page->m_prev;
            while (b != rhs.page)
            {
                be_assert(b->m_used % sizeof(EntityOperationRemove) == 0,
                          "invalid iterator difference");
                diff += b->m_used / sizeof(EntityOperationRemove);
            }
            be_assert((b->m_used - offset) % sizeof(EntityOperationRemove) == 0,
                      "invalid iterator difference");
            diff += (b->m_used - offset) / sizeof(EntityOperationRemove);
            return diff;
        }
        else
        {
            u32 byteDistance = offset - rhs.offset;
            be_assert(byteDistance % sizeof(EntityOperationRemove) == 0,
                      "invalid iterator difference");
            return byteDistance / sizeof(EntityOperationRemove);
        }
    }

    bool operator==(const EntityOperationRemoveIterator& other) const
    {
        return page == other.page && offset == other.offset;
    }

    bool operator!=(const EntityOperationRemoveIterator& other) const
    {
        return page != other.page || offset != other.offset;
    }
};

struct ComponentGroup::EntityOperationCompare
{
    bool operator()(const EntityOperationRemove& op1, const EntityOperationRemove& op2) const
    {
        return op1.componentIndex < op2.componentIndex;
    }
};

struct ComponentGroup::OperationDelta
{
    u32 added;
    u32 removed;
    ComponentGroup::OperationBuffer* pageAdd;
    ComponentGroup::OperationBuffer* pageRemove;
    u32 pageAddOffset;
    u32 pageRemoveOffset;
};

ComponentGroup::ComponentGroup(u32 index, u32 firstComponent,
                               const minitl::vector< raw<const RTTI::Class> >& componentTypes,
                               const minitl::vector<u32>& bucketMasks,
                               SystemAllocator& allocator)
    :   m_allocator(allocator)
    ,   m_buckets(Arena::game(), (u32)bucketMasks.size())
    ,   m_components(Arena::game(), (u32)componentTypes.size())
    ,   m_componentsTotalSize(0)
    ,   m_entityOperation(new (m_allocator.allocate()) OperationBuffer(0))
    ,   m_entityOperationCurrent(m_entityOperation)
    ,   firstComponent(firstComponent)
    ,   lastComponent(firstComponent + (u32)componentTypes.size())
    ,   index(index)
{
    u32 bucket = 0;
    for (minitl::vector<u32>::const_iterator it = bucketMasks.begin();
         it != bucketMasks.end();
         ++it, ++bucket)
    {
        u32 size = 0;
        for (u32 m = *it, i = 0; m; m >>= 1, ++i)
        {
            if (m & 1)
            {
                size += componentTypes[i]->size;
            }
        }
        m_buckets[bucket] = Bucket(*it, size);
    }
    minitl::vector< raw<const RTTI::Class> >::const_iterator begin = componentTypes.begin();
    for (minitl::vector< raw<const RTTI::Class> >::const_iterator it = begin;
        it != componentTypes.end();
        ++it)
    {
        ComponentInfo& info = m_components[be_checked_numcast<u32>(minitl::distance(begin, it))];
        info.componentType = *it;
        info.created = getMethodFromClass(*it, "created");
        info.destroyed = getMethodFromClass(*it, "destroyed");
        info.size = (*it)->size;
        m_componentsTotalSize += (*it)->size;
    }
}

ComponentGroup::~ComponentGroup()
{
}

ComponentGroup::BucketPair ComponentGroup::findBuckets(u32 mask1, u32 mask2)
{
    BucketPair result(0, 0);
    minitl::tuple<u32, u32> best;
    for (Bucket* bucket = m_buckets.begin(); bucket != m_buckets.end(); ++bucket)
    {
        if ((bucket->acceptMask & mask1) == bucket->acceptMask && bucket->maskSize >= best.first)
        {
            result.first = bucket;
            best.first = bucket->maskSize;
        }
        if ((bucket->acceptMask & mask2) == bucket->acceptMask && bucket->maskSize >= best.second)
        {
            result.second = bucket;
            best.second = bucket->maskSize;
        }
    }
    return result;
}

void ComponentGroup::unpackComponents(u32 mask, const byte source[],
                                      byte target[], const u32 offset[]) const
{
    for (u32 c = 0; mask; ++c, mask >>= 1)
    {
        if (mask & 1)
        {
            const u32 size = m_components[c].size;
            memcpy(target + offset[c], source, size);
            source += size;
        }
    }
}

void ComponentGroup::packComponents(u32 mask, const byte source[],
                                    byte target[], const u32 offset[]) const
{
    for (u32 c = 0; mask; ++c, mask >>= 1)
    {
        if (mask & 1)
        {
            const u32 size = m_components[c].size;
            memcpy(target, source + offset[c], size);
            target += size;
        }
    }
}

void ComponentGroup::copyComponents(weak<EntityStorage> storage, u32 owner, byte target[],
                                    u32 mask, const u32 offset[]) const
{
    Entity e = { owner };
    for (u32 c = 0; mask; ++c, mask >>= 1)
    {
        if (mask & 1)
        {
            storage->copyComponent(e, c + firstComponent, target + offset[c]);
        }
    }
}


struct MergeOperation
{
    const ComponentGroup* owner;
    ComponentGroup::EntityOperation* operation;
    u32 maskBefore;
    u32 maskAfter;
    u32 maskChanged;
    byte* componentBuffer;
    u32* componentOffsets;

    MergeOperation(const ComponentGroup* owner, ComponentGroup::EntityOperation* operation,
                   byte* componentBuffer, u32* componentOffsets)
        :   owner(owner)
        ,   operation(operation)
        ,   maskBefore(operation->maskBefore)
        ,   maskAfter(operation->maskAfter)
        ,   maskChanged(0)
        ,   componentBuffer(componentBuffer)
        ,   componentOffsets(componentOffsets)
    {
    }

    void operator()(ComponentGroup::OperationBuffer* /*oldBuffer*/,
                    ComponentGroup::OperationBuffer* /*newBuffer*/)
    {
    }

    void operator()(ComponentGroup::OperationBuffer* /*buffer*/, ComponentGroup::EntityOperation* target)
    {
        if (target->owner == operation->owner)
        {
            be_assert(maskAfter == target->maskBefore,
                      "incorrect sequence of operation");
            u32 maskRemoved = maskBefore & ~maskAfter;
            u32 maskAdded = target->maskAfter & ~maskAfter;
            maskChanged &= ~(maskAfter & ~target->maskAfter);
            maskChanged |= maskRemoved & maskAdded;
            maskAfter = target->maskAfter;
            target->owner = s_invalidEntity;
            byte* componentsBuffer = target->components();
            u32 maskComponents = target->maskAfter & ~target->maskBefore;
            owner->unpackComponents(maskComponents, componentsBuffer,
                                    componentBuffer, componentOffsets);
        }
    }
};

struct TryMergeOperation
{
    ComponentGroup* owner;
    weak<EntityStorage> storage;
    ComponentGroup::OperationDelta* deltas;
    byte* componentBuffer;
    u32* componentOffsets;
    TryMergeOperation(ComponentGroup* owner, weak<EntityStorage> storage,
                      ComponentGroup::OperationDelta deltas[],
                      byte* componentBuffer, u32* componentOffsets)
        :   owner(owner)
        ,   storage(storage)
        ,   deltas(deltas)
        ,   componentBuffer(componentBuffer)
        ,   componentOffsets(componentOffsets)
    {
    }

    void operator()(ComponentGroup::OperationBuffer* oldBuffer,
                    ComponentGroup::OperationBuffer* /*newBuffer*/)
    {
        oldBuffer->~OperationBuffer();
        owner->m_allocator.free(oldBuffer);
    }

    void operator()(ComponentGroup::OperationBuffer* buffer,
                    ComponentGroup::EntityOperation* operation)
    {
        if (operation->owner != s_invalidEntity)
        {
            typedef ComponentGroup::EntityOperation         EntityOperation;
            typedef ComponentGroup::EntityOperationRemove   EntityOperationRemove;
            Entity e = { operation->owner };
            EntityInfo& info = storage->getEntityInfo(e);
            owner->unpackComponents((operation->maskAfter & ~operation->maskBefore),
                                    operation->components(),
                                    componentBuffer, componentOffsets);
            MergeOperation m(owner, operation, componentBuffer, componentOffsets);
            buffer->forEach(operation->next(), m);

            minitl::tuple<Bucket*, Bucket*> buckets = owner->findBuckets(m.maskBefore, m.maskAfter);
            const u32 dummyOffset = be_checked_numcast<u32>(owner->m_buckets.end() - owner->m_buckets.begin());
            if (buckets.first != buckets.second)
            {
                owner->copyComponents(storage, operation->owner, componentBuffer,
                                      m.maskBefore & m.maskAfter & ~m.maskChanged,
                                      componentOffsets);

                if (buckets.first->acceptMask)
                {
                    void* memory = owner->allocOperation(sizeof(u32));
                    EntityOperationRemove* removeOperation = new (memory) EntityOperationRemove;
                    removeOperation->operation.owner = operation->owner;
                    removeOperation->operation.maskBefore = be_checked_numcast<u32>(buckets.first - owner->m_buckets.begin());
                    removeOperation->operation.maskAfter = dummyOffset;
                    removeOperation->operation.size = sizeof(EntityOperationRemove);
                    deltas[buckets.first - owner->m_buckets.begin()].removed++;
                    u32 absoluteComponentIndex = owner->firstComponent + buckets.first->firstComponent;
                    EntityInfo::BucketInfo& bucketInfo = info.buckets[absoluteComponentIndex];
                    be_assert(bucketInfo.bucket == buckets.first - owner->m_buckets.begin(),
                              "EntityInfo bucket does not match entity mask");
                    removeOperation->componentIndex = bucketInfo.offset;
                }

                if (buckets.second->acceptMask)
                {
                    u32 extraSize = buckets.second->componentSize;
                    EntityOperation* addOperation = new (owner->allocOperation(extraSize)) EntityOperation;
                    addOperation->owner = operation->owner;
                    addOperation->maskBefore = dummyOffset;
                    addOperation->maskAfter = be_checked_numcast<u32>(buckets.second - owner->m_buckets.begin());
                    addOperation->size = sizeof(EntityOperation) + extraSize;
                    deltas[buckets.second - owner->m_buckets.begin()].added++;
                    owner->packComponents(buckets.second->acceptMask, componentBuffer,
                                          addOperation->components(), componentOffsets);
                }
            }

            {
                Bucket* componentBucket = owner->m_buckets.end() - owner->m_components.size() - 1;
                u32 remainderMaskRemove = m.maskBefore & ~m.maskAfter & ~buckets.first->acceptMask;
                u32 remainderMaskAdd = m.maskAfter & ~m.maskBefore & ~buckets.second->acceptMask;
                u32 remainderMaskMoved = m.maskAfter & buckets.first->acceptMask & ~buckets.second->acceptMask;
                for (u32 i = 0;
                     remainderMaskRemove | remainderMaskAdd | remainderMaskMoved;
                     remainderMaskRemove >>= 1, remainderMaskAdd >>= 1, remainderMaskMoved >>= 1,
                     ++i, ++componentBucket)
                {
                    if (remainderMaskRemove & 1)
                    {
                        be_assert(componentBucket->acceptMask == (1u<<i),
                                  "invalid component bucket for component %d" | i);
                        void* memory = owner->allocOperation(sizeof(u32));
                        EntityOperationRemove* removeOperation = new (memory) EntityOperationRemove;
                        removeOperation->operation.owner = operation->owner;
                        removeOperation->operation.maskBefore = be_checked_numcast<u32>(componentBucket - owner->m_buckets.begin());
                        removeOperation->operation.maskAfter = dummyOffset;
                        removeOperation->operation.size = sizeof(EntityOperation) + sizeof(u32);
                        deltas[componentBucket - owner->m_buckets.begin()].removed++;
                        u32 absoluteComponentIndex = owner->firstComponent + i;
                        EntityInfo::BucketInfo& bucketInfo = info.buckets[absoluteComponentIndex];
                        be_assert(bucketInfo.bucket == componentBucket - owner->m_buckets.begin(),
                                  "EntityInfo bucket does not match entity mask");
                        removeOperation->componentIndex = bucketInfo.offset;
                    }
                    if (remainderMaskAdd & 1 || remainderMaskMoved & 1)
                    {
                        be_assert(componentBucket->acceptMask == (1u<<i),
                                  "invalid component bucket for component %d" | i);
                        EntityOperation* addOperation = new (owner->allocOperation(owner->m_components[i].size)) EntityOperation;
                        addOperation->owner = operation->owner;
                        addOperation->maskBefore = dummyOffset;
                        addOperation->maskAfter = be_checked_numcast<u32>(componentBucket - owner->m_buckets.begin());
                        addOperation->size = sizeof(EntityOperation) + owner->m_components[i].size;
                        owner->packComponents(componentBucket->acceptMask, componentBuffer,
                                              addOperation->components(), componentOffsets);
                        deltas[componentBucket - owner->m_buckets.begin()].added++;
                    }
                }
            }
        }
    }
};

void ComponentGroup::groupEntityOperations(weak<EntityStorage> storage, OperationDelta deltas[])
{
    OperationBuffer* oldBuffer = m_entityOperation;
    m_entityOperation = new (m_allocator.allocate()) OperationBuffer(0);
    m_entityOperationCurrent = m_entityOperation;
    minitl::Allocator::Block<byte> componentBuffer(Arena::stack(), m_componentsTotalSize);
    minitl::Allocator::Block<u32> componentOffsets(Arena::stack(), m_components.size());
    for (u32 o = 0, i = 0; i < m_components.size(); o += m_components[i].size, ++i)
    {
        componentOffsets[i] = o;
    }

    TryMergeOperation merge(this, storage, deltas, componentBuffer.begin(), componentOffsets.begin());
    oldBuffer->forEach(merge);
}

ComponentGroup::OperationBuffer* ComponentGroup::sortEntityOperations(OperationDelta deltas[])
{
    OperationBuffer* firstBuffer = new (m_allocator.allocate()) OperationBuffer(0);
    OperationBuffer* current = firstBuffer;
    const u32 bufferSpace = m_allocator.blockSize() - sizeof(OperationBuffer) + 1;
    for (u32 i = 0; i < m_buckets.size(); ++i)
    {
        {
            if (deltas[i].added)
            {
                u32 operationAddSize = (u32)sizeof(EntityOperation) + m_buckets[i].componentSize;
                u32 maxOps = (bufferSpace - current->m_used) / operationAddSize;
                u32 placedOp = minitl::min(maxOps, deltas[i].added);
                deltas[i].pageAdd = current;
                deltas[i].pageAddOffset = current->m_used;
                current->m_used += placedOp * operationAddSize;
                for (u32 opCount = placedOp; opCount != deltas[i].added; /*nothing*/)
                {
                    OperationBuffer* newBuffer = new (m_allocator.allocate()) OperationBuffer(current);
                    current->m_next = newBuffer;
                    current = newBuffer;
                    u32 maxOpsNext = (bufferSpace - current->m_used) / operationAddSize;
                    u32 placedOpNext = minitl::min(maxOpsNext, deltas[i].added-opCount);
                    current->m_used += placedOpNext * operationAddSize;
                    opCount += placedOpNext;
                }
            }
            else
            {
                deltas[i].pageAdd = current;
                deltas[i].pageAddOffset = current->m_used;
            }
        }

        {
            if (deltas[i].removed)
            {
                u32 operationRemoveSize = (u32)sizeof(EntityOperation) + sizeof(u32);
                u32 maxOps = (bufferSpace - current->m_used) / operationRemoveSize;
                u32 placedOp = minitl::min(maxOps, deltas[i].removed);
                deltas[i].pageRemove = current;
                deltas[i].pageRemoveOffset = current->m_used;
                current->m_used += placedOp * operationRemoveSize;
                for (u32 opCount = placedOp; opCount != deltas[i].removed; /*nothing*/)
                {
                    OperationBuffer* newBuffer = new (m_allocator.allocate()) OperationBuffer(current);
                    current->m_next = newBuffer;
                    current = newBuffer;
                    u32 maxOpsNext = (bufferSpace - current->m_used) / operationRemoveSize;
                    u32 placedOpNext = minitl::min(maxOpsNext, deltas[i].removed-opCount);
                    current->m_used += placedOpNext * operationRemoveSize;
                    opCount += placedOpNext;
                }
            }
            else
            {
                deltas[i].pageRemove = current;
                deltas[i].pageRemoveOffset = current->m_used;
            }
        }
    }

    typedef minitl::tuple<OperationBuffer*, u32> CurrentBufferOffset;
    minitl::Allocator::Block<CurrentBufferOffset> addOffsets(Arena::stack(), m_buckets.size());
    minitl::Allocator::Block<CurrentBufferOffset> removeOffsets(Arena::stack(), m_buckets.size());
    for (u32 i = 0; i < m_buckets.size(); ++i)
    {
        be_assert(deltas[i].pageAdd != 0, "invalid page 0x0 for add bucket %d" | i);
        be_assert(deltas[i].pageRemove != 0, "invalid page 0x0 for remove bucket %d" | i);
        addOffsets[i].first = deltas[i].pageAdd;
        addOffsets[i].second = deltas[i].pageAddOffset;
        removeOffsets[i].first = deltas[i].pageRemove;
        removeOffsets[i].second = deltas[i].pageRemoveOffset;
    }

    const u32 dummyOffset = be_checked_numcast<u32>(m_buckets.end() - m_buckets.begin());
    for (OperationBuffer* sourceBuffer = m_entityOperation;
         sourceBuffer;
         sourceBuffer = sourceBuffer->m_next)
    {
        EntityOperation* last = (EntityOperation*)(sourceBuffer->m_data + sourceBuffer->m_used);
        for (EntityOperation* operation = (EntityOperation*)sourceBuffer->m_data;
             operation < last;
             operation = operation->next())
        {
            CurrentBufferOffset& offset = operation->maskBefore == dummyOffset
                                        ? addOffsets[operation->maskAfter]
                                        : removeOffsets[operation->maskBefore];
            u32 size = operation->size;
            memcpy (offset.first->m_data + offset.second,
                    operation,
                    size);
            offset.second += size;
            be_assert(offset.second <= offset.first->m_used,
                      "offsets for bucket is invalid");
            if (offset.second == offset.first->m_used)
            {
                offset.first = offset.first->m_next;
                offset.second = 0;
            }
        }
    }

    for (u32 i = 0; i < m_buckets.size() - 1; ++i)
    {
        EntityOperationRemoveIterator begin = { deltas[i].pageRemove, deltas[i].pageRemoveOffset };
        EntityOperationRemoveIterator end = { deltas[i+1].pageAdd, deltas[i+1].pageAddOffset };
        minitl::sort(begin, end, EntityOperationCompare());
    }

    return firstBuffer;
}

void ComponentGroup::repack(weak<EntityStorage> storage, u32 componentIndex, Bucket& bucket,
                            u32 firstEntity, u32 firstEntityStorage, OperationDelta operations,
                            i32 offset) const
{
    const u32 absoluteComponentIndex = componentIndex + firstComponent;
    u32 componentOffset = 0;
    if (absoluteComponentIndex != lastComponent)
    {
        componentOffset += sizeof(EntityOperation);
        for (u32 i = 0, mask = bucket.acceptMask;
             i < componentIndex;
             ++i, mask >>= 1)
        {
            if (mask & 1)
            {
                componentOffset += m_components[i].size;
            }
        }
    }
    EntityStorage::ComponentStorage& componentStorage = absoluteComponentIndex != lastComponent
                                                      ? *storage->m_components[absoluteComponentIndex]
                                                      : *storage->m_componentBackLinks[index];
    EntityStorage::ComponentStorage& backlinkStorage = *storage->m_componentBackLinks[index];
    const u32 componentSize = absoluteComponentIndex != lastComponent
                            ? m_components[componentIndex].size
                            : sizeof(u32);
    const u32 entityCount = bucket.entityCount;
    const u32 newFirstEntity = firstEntity + offset;
    const u32 newEntityCount = bucket.entityCount + operations.added - operations.removed;
    const u32 entitiesToMove = minitl::min(entityCount, newEntityCount);
    u32 removeOperationCount = operations.removed;
    u32 currentRemoveOperationFront = 0;
    OperationBuffer* bufferRemove = operations.pageRemove;
    u32 bufferRemoveOffset = operations.pageRemoveOffset;

    /* repacks components at back in the middle, freeing space at back */
    if (newEntityCount < entityCount)
    {
        u32 currentRemoveOperationBack = removeOperationCount - 1;
        OperationBuffer* lastRemoveOpBuffer = operations.pageRemove;
        u32 lastRemoveOpOffset = operations.pageRemoveOffset + currentRemoveOperationBack * sizeof(EntityOperationRemove);
        if (lastRemoveOpOffset > lastRemoveOpBuffer->m_used)
        {
            lastRemoveOpOffset -= operations.pageRemoveOffset;
            lastRemoveOpBuffer = lastRemoveOpBuffer->m_next;
        }
        while (lastRemoveOpOffset > lastRemoveOpBuffer->m_used)
        {
            lastRemoveOpOffset -= lastRemoveOpBuffer->m_used;
            lastRemoveOpBuffer = lastRemoveOpBuffer->m_next;
        }
        const EntityOperationRemove* firstRemoveOp = reinterpret_cast<const EntityOperationRemove*>(bufferRemove->m_data + bufferRemoveOffset);
        be_assert(firstRemoveOp->operation.maskBefore == (u32)(&bucket - m_buckets.begin()),
                  "Operation buffer inconsistency");
        const EntityOperationRemove* lastRemoveOp =  reinterpret_cast<const EntityOperationRemove*>(lastRemoveOpBuffer->m_data + lastRemoveOpOffset);
        be_assert(lastRemoveOp->operation.maskBefore == (u32)(&bucket - m_buckets.begin()),
                  "Operation buffer inconsistency");
        for (u32 entityIndex = entityCount;
             entityIndex > newEntityCount;
             --entityIndex)
        {
            if (entityIndex-1 == lastRemoveOp->componentIndex)
            {
                --removeOperationCount;
                if (lastRemoveOp == firstRemoveOp)
                {
                    break;
                }
                else if (lastRemoveOpOffset > 0)
                {
                    lastRemoveOpOffset -= sizeof(EntityOperationRemove);
                }
                else
                {
                    lastRemoveOpBuffer = lastRemoveOpBuffer->m_prev;
                    lastRemoveOpOffset = lastRemoveOpBuffer->m_used - sizeof(EntityOperationRemove);
                }
                lastRemoveOp =  reinterpret_cast<const EntityOperationRemove*>(lastRemoveOpBuffer->m_data + lastRemoveOpOffset);
                be_assert(lastRemoveOp->operation.maskBefore == (u32)(&bucket - m_buckets.begin()),
                          "Operation buffer inconsistency");
            }
            else
            {
                Entity e = { *reinterpret_cast<u32*>(backlinkStorage.getElement(firstEntityStorage + entityIndex - 1)) };
                EntityInfo& info = storage->getEntityInfo(e);
                u32 pageSrc = (firstEntity + entityIndex - 1) / componentStorage.elementsPerPage;
                u32 offsetSrc = (firstEntity + entityIndex - 1) % componentStorage.elementsPerPage;
                u32 pageDst = (firstEntity + firstRemoveOp->componentIndex) / componentStorage.elementsPerPage;
                u32 offsetDst = (firstEntity + firstRemoveOp->componentIndex) % componentStorage.elementsPerPage;
                if (absoluteComponentIndex != lastComponent)
                {
                    be_assert(info.buckets[absoluteComponentIndex].group == index,
                              "inconsistend backlink buffer");
                    be_assert(info.buckets[absoluteComponentIndex].bucket == be_checked_numcast<u16>(&bucket-m_buckets.begin()),
                              "inconsistend backlink buffer");
                    be_assert(info.buckets[absoluteComponentIndex].offset == entityIndex-1,
                              "inconsistend backlink buffer");
                    info.buckets[absoluteComponentIndex].offset = firstRemoveOp->componentIndex;
                }
                memcpy(componentStorage.pages[pageDst] + offsetDst*componentSize,
                       componentStorage.pages[pageSrc] + offsetSrc*componentSize,
                       componentSize);

                --removeOperationCount;
                if (lastRemoveOp == firstRemoveOp)
                {
                    break;
                }
                else
                {
                    bufferRemoveOffset += sizeof(EntityOperationRemove);
                    if (bufferRemoveOffset >= bufferRemove->m_used)
                    {
                        bufferRemoveOffset = 0;
                        bufferRemove = bufferRemove->m_next;
                    }
                    ++currentRemoveOperationFront;
                    firstRemoveOp = reinterpret_cast<const EntityOperationRemove*>(bufferRemove->m_data + bufferRemoveOffset);
                    be_assert(firstRemoveOp->operation.maskBefore == (u32)(&bucket - m_buckets.begin()),
                              "Operation buffer inconsistency");
                }
            }
        }
    }

    u32 pageSrcStart = firstEntity / componentStorage.elementsPerPage;
    u32 pageSrcEnd = (firstEntity + entitiesToMove) / componentStorage.elementsPerPage;
    u32 countSrcStart = firstEntity % componentStorage.elementsPerPage;
    u32 countSrcEnd = (firstEntity + entitiesToMove)  % componentStorage.elementsPerPage;

    u32 pageDstStart = newFirstEntity / componentStorage.elementsPerPage;
    u32 pageDstEnd = (newFirstEntity + entitiesToMove) / componentStorage.elementsPerPage;
    u32 countDstStart = newFirstEntity % componentStorage.elementsPerPage;
    u32 countDstEnd = (newFirstEntity + entitiesToMove) % componentStorage.elementsPerPage;

    /* move compact buffer in place */
    if (offset < 0 || (offset > 0 && offset >= (i32)entityCount))
    {
        while (pageSrcStart != pageSrcEnd || countSrcStart != countSrcEnd)
        {
            memcpy(componentStorage.pages[pageDstStart] + countDstStart*componentSize,
                   componentStorage.pages[pageSrcStart] + countSrcStart*componentSize,
                   componentSize);

            if (countSrcStart < componentStorage.elementsPerPage-1)
            {
                ++countSrcStart;
            }
            else
            {
                countSrcStart = 0;
                ++pageSrcStart;
            }

            if (countDstStart < componentStorage.elementsPerPage-1)
            {
                ++countDstStart;
            }
            else
            {
                countDstStart = 0;
                ++pageDstStart;
            }
        }
    }
    else if (offset > 0)
    {
        while (pageSrcStart != pageSrcEnd || countSrcStart != countSrcEnd)
        {
            if (countSrcEnd > 0)
            {
                --countSrcEnd;
            }
            else
            {
                countSrcEnd = componentStorage.elementsPerPage-1;
                --pageSrcEnd;
            }

            if (countDstEnd > 0)
            {
                --countDstEnd;
            }
            else
            {
                countDstEnd = componentStorage.elementsPerPage-1;
                --pageDstEnd;
            }
            memcpy(componentStorage.pages[pageDstEnd] + countDstEnd*componentSize,
                   componentStorage.pages[pageSrcEnd] + countSrcEnd*componentSize,
                   componentSize);
        }
    }

    /* replace deleted entities with added entities */
    OperationBuffer* bufferAdd = operations.pageAdd;
    u32 bufferAddOffset = operations.pageAddOffset;
    u32 consumedOps = 0;
    for (/*nothing*/;
         consumedOps < minitl::min(operations.added, removeOperationCount);
         ++consumedOps)
    {
        byte* buffer = bufferRemove->m_data + bufferRemoveOffset;
        const EntityOperationRemove* remove = reinterpret_cast<const EntityOperationRemove*>(buffer);
        be_assert(remove->operation.maskBefore == (u32)(&bucket - m_buckets.begin()),
                  "Operation buffer inconsistency");
        be_assert (remove->componentIndex < entitiesToMove,
                   "mismatch added/removed ops");
        buffer = bufferAdd->m_data + bufferAddOffset;
        const EntityOperation* add = reinterpret_cast<const EntityOperation*>(buffer);
        be_assert(add->maskAfter == (u32)(&bucket - m_buckets.begin()),
                  "Operation buffer inconsistency");
        Entity e = { add->owner };
        EntityInfo& info = storage->getEntityInfo(e);

        u32 pageDst = (newFirstEntity + remove->componentIndex) / componentStorage.elementsPerPage;
        u32 offsetDst = (newFirstEntity + remove->componentIndex) % componentStorage.elementsPerPage;

        memcpy(componentStorage.pages[pageDst] + offsetDst*componentSize,
               reinterpret_cast<const byte*>(add) + componentOffset,
               componentSize);
        if (absoluteComponentIndex != lastComponent)
        {
            be_assert(info.buckets[absoluteComponentIndex].group == index,
                      "inconsistend backlink buffer");
            info.buckets[absoluteComponentIndex].bucket = be_checked_numcast<u16>(add->maskAfter);
            info.buckets[absoluteComponentIndex].offset = remove->componentIndex;
        }
        bufferAddOffset += add->size;
        if (bufferAddOffset >= bufferAdd->m_used)
        {
            bufferAdd = bufferAdd->m_next;
            bufferAddOffset = 0;
        }
        bufferRemoveOffset += sizeof(EntityOperationRemove);
        if (bufferRemoveOffset >= bufferRemove->m_used)
        {
            bufferRemove = bufferRemove->m_next;
            bufferRemoveOffset = 0;
        }
    }

    for (u32 i = 0; i < operations.added - consumedOps; ++i)
    {
        u32 destIndex = entitiesToMove + i;
        byte* buffer = bufferAdd->m_data + bufferAddOffset;
        const EntityOperation* add = reinterpret_cast<const EntityOperation*>(buffer);
        be_assert(add->maskAfter == (u32)(&bucket - m_buckets.begin()),
                  "Operation buffer inconsistency");
        Entity e = {add->owner};
        EntityInfo& info = storage->getEntityInfo(e);

        u32 pageDst = (newFirstEntity + destIndex) / componentStorage.elementsPerPage;
        u32 offsetDst = (newFirstEntity + destIndex) % componentStorage.elementsPerPage;

        memcpy(componentStorage.pages[pageDst] + offsetDst*componentSize,
               reinterpret_cast<const byte*>(add) + componentOffset,
               componentSize);
        bufferAddOffset += add->size;
        if (bufferAddOffset >= bufferAdd->m_used)
        {
            bufferAdd = bufferAdd->m_next;
            bufferAddOffset = 0;
        }
        if (absoluteComponentIndex != lastComponent)
        {
            be_assert(info.buckets[absoluteComponentIndex].group == index,
                      "inconsistend backlink buffer");
            info.buckets[absoluteComponentIndex].bucket = be_checked_numcast<u16>(add->maskAfter);
            info.buckets[absoluteComponentIndex].offset = destIndex;
        }
    }
}

void ComponentGroup::executeEntityOperations(weak<EntityStorage> storage,
                                             const OperationDelta deltas[])
{
    minitl::Allocator::Block< minitl::tuple<i32, u32, u32> > offsets(Arena::stack(), m_buckets.size()-1);
    for (u32 componentIndex = 0; componentIndex < m_components.size(); ++componentIndex)
    {
        u32 absoluteComponentIndex = componentIndex + firstComponent;
        EntityStorage::ComponentStorage& componentStorage = *storage->m_components[absoluteComponentIndex];
        const u32 mask = 1 << componentIndex;
        memset(offsets.begin(), 0, be_checked_numcast<size_t>(offsets.byteCount()));
        i32 absoluteOffset = 0;
        u32 entityCount = 0;
        u32 entityCountAll = 0;
        for (u32 bucket = 0; bucket < m_buckets.size()-1; ++bucket)
        {
            offsets[bucket].first = absoluteOffset;
            offsets[bucket].second = entityCount;
            offsets[bucket].third = entityCountAll;
            if (m_buckets[bucket].acceptMask & mask)
            {
                absoluteOffset += deltas[bucket].added - deltas[bucket].removed;
                entityCount += m_buckets[bucket].entityCount;
            }
            entityCountAll += m_buckets[bucket].entityCount;
        }
        if (absoluteOffset > 0)
        {
            componentStorage.reserve(absoluteOffset);
        }
        minitl::array<u32> bucketsToProcess(Arena::stack(), m_buckets.size()-1);
        minitl::array<bool> bucketsProcessed(Arena::stack(), m_buckets.size()-1);
        for (u32 i = 0; i < bucketsProcessed.size(); ++i)
        {
            bucketsProcessed[i] = false;
        }
        for (u32 done = 0; done != bucketsToProcess.size(); /*nothing*/)
        {
            for (u32 i = 0; i < m_buckets.size()-1; ++i)
            {
                if (!bucketsProcessed[i])
                {
                    bool spaceLeft = i == 0
                                  || bucketsProcessed[i-1]
                                  || offsets[i].first >= 0;
                    bool spaceRight = i == m_buckets.size()-2
                                   || bucketsProcessed[i+1]
                                   || offsets[i+1].first <= 0;
                    if (spaceLeft && spaceRight)
                    {
                        bucketsToProcess[done++] = i;
                        bucketsProcessed[i] = true;
                    }
                }
            }
            for (u32 i = bucketsToProcess.size(); i > 0; --i)
            {
                if (!bucketsProcessed[i-1])
                {
                    bool spaceLeft = i == 1
                                  || bucketsProcessed[i-2]
                                  || offsets[i-1].first >= 0;
                    bool spaceRight = i == m_buckets.size()-1
                                   || bucketsProcessed[i]
                                   || offsets[i].first <= 0;
                    if (spaceLeft && spaceRight)
                    {
                        bucketsToProcess[done++] = i-1;
                        bucketsProcessed[i-1] = true;
                    }
                }
            }
        }
        for (u32 i = 0; i < bucketsToProcess.size(); ++i)
        {
            u32 bucketToProcess = bucketsToProcess[i];
            be_assert(bucketToProcess < m_buckets.size() - 1,
                      "should not process last bucket");
            if (m_buckets[bucketToProcess].acceptMask & mask)
            {
                repack(storage, componentIndex, m_buckets[bucketToProcess],
                       offsets[bucketToProcess].second, offsets[bucketToProcess].third,
                       deltas[bucketToProcess], offsets[bucketToProcess].first);
            }
        }
        if (absoluteOffset < 0)
        {
            componentStorage.shrink(absoluteOffset);
        }
    }

    {
        i32 absoluteOffset = 0;
        u32 entityCount = 0;
        for (u32 bucket = 0; bucket < m_buckets.size()-1; ++bucket)
        {
            offsets[bucket].first = absoluteOffset;
            offsets[bucket].second = entityCount;
            offsets[bucket].third = entityCount;
            absoluteOffset += deltas[bucket].added - deltas[bucket].removed;
            entityCount += m_buckets[bucket].entityCount;
        }
        if (absoluteOffset > 0)
        {
            storage->m_componentBackLinks[index]->reserve(absoluteOffset);
        }

        minitl::array<u32> bucketsToProcess(Arena::stack(), m_buckets.size()-1);
        minitl::array<bool> bucketsProcessed(Arena::stack(), m_buckets.size()-1);
        for (u32 i = 0; i < bucketsProcessed.size(); ++i)
        {
            bucketsProcessed[i] = false;
        }
        for (u32 done = 0; done != bucketsToProcess.size(); /*nothing*/)
        {
            for (u32 i = 0; i < m_buckets.size() - 1; ++i)
            {
                if (!bucketsProcessed[i])
                {
                    bool spaceLeft = i == 0
                                  || bucketsProcessed[i-1]
                                  || offsets[i].first >= 0;
                    bool spaceRight = i == m_buckets.size()-2
                                   || bucketsProcessed[i+1]
                                   || offsets[i+1].first <= 0;
                    if (spaceLeft && spaceRight)
                    {
                        bucketsToProcess[done++] = i;
                        bucketsProcessed[i] = true;
                    }
                }
            }
            for (u32 i = m_buckets.size() - 1; i > 0; --i)
            {
                if (!bucketsProcessed[i-1])
                {
                    bool spaceLeft = i == 1
                                  || bucketsProcessed[i-2]
                                  || offsets[i-1].first >= 0;
                    bool spaceRight = i == m_buckets.size() - 1
                                   || bucketsProcessed[i]
                                   || offsets[i].first <= 0;
                    if (spaceLeft && spaceRight)
                    {
                        bucketsToProcess[done++] = i-1;
                        bucketsProcessed[i-1] = true;
                    }
                }
            }
        }
        for (u32 i = 0; i < bucketsToProcess.size(); ++i)
        {
            u32 bucketToProcess = bucketsToProcess[i];
            be_assert(bucketToProcess < m_buckets.size() - 1,
                      "should not process last bucket");
            repack(storage, lastComponent - firstComponent, m_buckets[bucketToProcess],
                   offsets[bucketToProcess].second, offsets[bucketToProcess].third,
                   deltas[bucketToProcess], offsets[bucketToProcess].first);
        }
        for (u32 i = 0; i < m_buckets.size(); ++i)
        {
            m_buckets[i].entityCount += deltas[i].added - deltas[i].removed;
        }
        if (absoluteOffset < 0)
        {
            storage->m_componentBackLinks[index]->shrink(absoluteOffset);
        }
    }
}

void ComponentGroup::clearBuffers(OperationBuffer *head) const
{
    while (head)
    {
        OperationBuffer* next = head->m_next;
        head->~OperationBuffer();
        m_allocator.free(head);
        head = next;
    }
}

void ComponentGroup::runEntityOperations(weak<EntityStorage> storage)
{
    minitl::Allocator::Block<OperationDelta> deltas(Arena::stack(), m_buckets.size());
    memset(deltas.begin(), 0, sizeof(OperationDelta) * m_buckets.size());
    groupEntityOperations(storage, deltas.begin());
    OperationBuffer* newBuffer = sortEntityOperations(deltas.begin());

    clearBuffers(m_entityOperation->m_next);
    m_entityOperation->m_next = 0;
    m_entityOperation->m_offset = 0;
    m_entityOperation->m_used = 0;
    m_entityOperationCurrent = m_entityOperation;
    executeEntityOperations(storage, deltas.begin());
    clearBuffers(newBuffer);
}

void ComponentGroup::freeBuffers()
{
    be_assert(m_entityOperation->m_used == 0 && m_entityOperation->m_next == 0,
              "Entity operation still in use when ComponentGroup is destroyed");
    m_entityOperation->~OperationBuffer();
    m_allocator.free(m_entityOperation);
}

void ComponentGroup::addComponent(Entity e, u32 originalMask,
                                  const Component& c, u32 componentIndex)
{
    const ComponentInfo& info = m_components[componentIndex];
    byte* operationBuffer = allocOperation(info.size);
    EntityOperation* operation = reinterpret_cast<EntityOperation*>(operationBuffer);
    operation->owner = e.id;
    operation->maskBefore = originalMask;
    operation->maskAfter = originalMask | (1 << componentIndex);
    operation->size = static_cast<u32>(sizeof(EntityOperation) + info.size);
    memcpy(operation->components(), &c, info.size);
}

void ComponentGroup::removeComponent(Entity e, u32 originalMask, u32 componentIndex)
{
    byte* operationBuffer = allocOperation(0);
    EntityOperation* operation = reinterpret_cast<EntityOperation*>(operationBuffer);
    operation->owner = e.id;
    operation->maskBefore = originalMask;
    operation->maskAfter = originalMask & ~(1 << componentIndex);
    operation->size = static_cast<u32>(sizeof(EntityOperation));
}

byte* ComponentGroup::allocOperation(u32 componentSize)
{
    const u32 totalSize = minitl::align(componentSize, sizeof(u32)) + sizeof(EntityOperation);
    do
    {
        OperationBuffer* buffer = m_entityOperationCurrent;
        u32 offset = buffer->m_offset.addExchange(totalSize);
        if (offset + totalSize + sizeof(OperationBuffer) > 256) //m_allocator.blockSize())
        {
            OperationBuffer* newBuffer = new (m_allocator.allocate()) OperationBuffer(buffer);
            if (m_entityOperationCurrent.setConditional(newBuffer, buffer) != buffer)
            {
                newBuffer->~OperationBuffer();
                m_allocator.free(newBuffer);
            }
            else
            {
                buffer->m_next = newBuffer;
            }
        }
        else
        {
            buffer->m_used += totalSize;
            return buffer->m_data + offset;
        }
    } while (true);
}

}}
