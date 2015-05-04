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
    for (raw<const RTTI::Method::Overload> overload = method->overloads;
         overload;
         overload = overload->next)
    {
        RTTI::Type componentType = RTTI::Type::makeType(type, RTTI::Type::Value, RTTI::Type::Mutable,
                                            RTTI::Type::Mutable);
        if (overload->parameterCount == 2
         && componentType.isA(overload->params->type)
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
        return reinterpret_cast<EntityOperationRemove*>(page->m_data + offset);
    }

    EntityOperationRemove& operator*() const
    {
        return *reinterpret_cast<EntityOperationRemove*>(page->m_data + offset);
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

    EntityOperationRemoveIterator operator+(u32 delta) const
    {
        EntityOperationRemoveIterator result = { page, offset };
        u32 byteOffset = delta * sizeof(EntityOperationRemove);
        while (result.offset + byteOffset > result.page->m_used)
        {
            byteOffset -= (result.page->m_used - result.offset);
            result.page = result.page->m_next;
            result.offset = 0;
        }
        return result;
    }

    EntityOperationRemoveIterator operator-(u32 delta) const
    {
        EntityOperationRemoveIterator result = { page, offset };
        u32 byteOffset = delta * sizeof(EntityOperationRemove);
        while (byteOffset > result.offset)
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

ComponentGroup::ComponentGroup(u32 firstComponent,
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
{
    u32 index = 0;
    for (minitl::vector<u32>::const_iterator it = bucketMasks.begin();
         it != bucketMasks.end();
         ++it, ++index)
    {
        u32 size = 0;
        for (u32 m = *it, i = 0; m; m >>= 1, ++i)
        {
            if (m & 1)
            {
                size += componentTypes[i]->size;
            }
        }
        m_buckets[index] = Bucket(*it, size);
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
    for (OperationBuffer* buffer = oldBuffer; buffer; /* nothing */)
    {
        EntityOperation* last = (EntityOperation*)(buffer->m_data + buffer->m_used);
        for (EntityOperation* operation = (EntityOperation*)buffer->m_data;
             operation < last;
             operation = operation->next())
        {
            if (operation->owner != s_invalidEntity)
            {
                Entity e = { operation->owner };
                EntityInfo& info = storage->getEntityInfo(e);
                u32 maskBefore = operation->maskBefore;
                u32 maskAfter = operation->maskAfter;
                u32 maskChanged = 0;
                byte* components = ((byte*)operation) + sizeof(EntityOperation);
                unpackComponents((maskAfter & ~maskBefore)|maskChanged, components,
                                 componentBuffer.begin(), componentOffsets.begin());
                for (EntityOperation* sibling = operation->next();
                     sibling < last;
                     sibling = sibling->next())
                {
                    if (sibling->owner == operation->owner)
                    {
                        be_assert(maskAfter == sibling->maskBefore,
                                  "incorrect sequence of operation");
                        u32 maskRemoved = maskBefore & ~maskAfter;
                        u32 maskAdded = sibling->maskAfter & ~maskAfter;
                        maskChanged &= ~(maskAfter & ~sibling->maskAfter);
                        maskChanged |= maskRemoved & maskAdded;
                        maskAfter = sibling->maskAfter;
                        sibling->owner = s_invalidEntity;
                        byte* components = (byte*)sibling + sizeof(EntityOperation);
                        u32 maskComponents = sibling->maskAfter & ~sibling->maskBefore;
                        unpackComponents(maskComponents, components,
                                         componentBuffer.begin(), componentOffsets.begin());
                    }
                }
                for (OperationBuffer* siblingBuffer = buffer->m_next;
                     siblingBuffer;
                     siblingBuffer = siblingBuffer->m_next)
                {
                    EntityOperation* last = (EntityOperation*)(siblingBuffer->m_data + siblingBuffer->m_used);
                    for (EntityOperation* sibling = (EntityOperation*)siblingBuffer->m_data;
                         sibling < last;
                         sibling = sibling->next())
                    {
                        if (sibling->owner == operation->owner)
                        {
                            be_assert(maskAfter == sibling->maskBefore,
                                      "incorrect sequence of operation");
                            u32 maskRemoved = maskBefore & ~maskAfter;
                            u32 maskAdded = sibling->maskAfter & ~maskAfter;
                            maskChanged &= ~(maskAfter & ~sibling->maskAfter);
                            maskChanged |= maskRemoved & maskAdded;
                            maskAfter = sibling->maskAfter;
                            sibling->owner = s_invalidEntity;
                            byte* components = (byte*)sibling + sizeof(EntityOperation);
                            u32 maskComponents = sibling->maskAfter & ~sibling->maskBefore;
                            unpackComponents(maskComponents, components,
                                             componentBuffer.begin(), componentOffsets.begin());
                        }
                    }
                }

                minitl::tuple<Bucket*, Bucket*> buckets = findBuckets(maskBefore, maskAfter);
                const u32 dummyOffset = m_buckets.end() - m_buckets.begin();
                if (buckets.first != buckets.second)
                {
                    copyComponents(storage, operation->owner, componentBuffer.begin(),
                                   maskBefore & maskAfter & ~maskChanged, componentOffsets.begin());

                    if (buckets.first->acceptMask)
                    {
                        void* memory = allocOperation(sizeof(u32));
                        EntityOperationRemove* removeOperation = new (memory) EntityOperationRemove;
                        removeOperation->operation.owner = operation->owner;
                        removeOperation->operation.maskBefore = buckets.first - m_buckets.begin();
                        removeOperation->operation.maskAfter = dummyOffset;
                        removeOperation->operation.size = sizeof(EntityOperationRemove);
                        deltas[buckets.first - m_buckets.begin()].removed++;
                        u32 absoluteComponentIndex = firstComponent + buckets.first->firstComponent;
                        EntityInfo::BucketInfo& bucketInfo = info.buckets[absoluteComponentIndex];
                        be_assert(bucketInfo.bucket == buckets.first - m_buckets.begin(),
                                  "EntityInfo bucket does not match entity mask");
                        removeOperation->componentIndex = bucketInfo.offset;
                    }

                    u32 extraSize = buckets.second->componentSize;
                    EntityOperation* addOperation = new (allocOperation(extraSize)) EntityOperation;
                    addOperation->owner = operation->owner;
                    addOperation->maskBefore = dummyOffset;
                    addOperation->maskAfter = buckets.second - m_buckets.begin();
                    addOperation->size = sizeof(EntityOperation) + extraSize;
                    deltas[buckets.second - m_buckets.begin()].added++;
                    packComponents(addOperation->maskAfter, componentBuffer.begin(),
                                   (byte*)addOperation + sizeof(EntityOperation), componentOffsets);
                }

                {
                    Bucket* componentBucket = m_buckets.end() - m_components.size() - 1;
                    u32 remainderMaskRemove = maskBefore & ~maskAfter & ~buckets.first->acceptMask;
                    u32 remainderMaskAdd = maskAfter & ~maskBefore & ~buckets.second->acceptMask;
                    for (u32 i = 0;
                         remainderMaskRemove | remainderMaskAdd;
                         remainderMaskRemove <<= 1, remainderMaskAdd <<= 1, ++i, ++componentBucket)
                    {
                        if (remainderMaskRemove & 1)
                        {
                            be_assert(componentBucket->acceptMask == (1<<i),
                                      "invalid component bucket for component %d" | i);
                            void* memory = allocOperation(sizeof(u32));
                            EntityOperationRemove* removeOperation = new (memory) EntityOperationRemove;
                            removeOperation->operation.owner = operation->owner;
                            removeOperation->operation.maskBefore = componentBucket - m_buckets.begin();
                            removeOperation->operation.maskAfter = dummyOffset;
                            removeOperation->operation.size = sizeof(EntityOperation) + sizeof(u32);
                            deltas[componentBucket - m_buckets.begin()].removed++;
                            u32 absoluteComponentIndex = firstComponent + i;
                            EntityInfo::BucketInfo& bucketInfo = info.buckets[absoluteComponentIndex];
                            be_assert(bucketInfo.bucket == componentBucket - m_buckets.begin(),
                                      "EntityInfo bucket does not match entity mask");
                            removeOperation->componentIndex = bucketInfo.offset;
                        }
                        if (remainderMaskAdd & 1)
                        {
                            be_assert(componentBucket->acceptMask == (1<<i),
                                      "invalid component bucket for component %d" | i);
                            EntityOperation* addOperation = new (allocOperation(m_components[i].size)) EntityOperation;
                            addOperation->owner = operation->owner;
                            addOperation->maskBefore = dummyOffset;
                            addOperation->maskAfter = componentBucket - m_buckets.begin();
                            addOperation->size = sizeof(EntityOperation) + m_components[i].size;
                            packComponents(addOperation->maskAfter, componentBuffer.begin(),
                                           (byte*)addOperation + sizeof(EntityOperation),
                                           componentOffsets);
                            deltas[componentBucket - m_buckets.begin()].added++;
                        }
                    }
                }
            }
        }
        OperationBuffer* next = buffer->m_next;
        buffer->~OperationBuffer();
        m_allocator.free(buffer);
        buffer = next;
    }
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
                    u32 maxOps = (bufferSpace - current->m_used) / operationAddSize;
                    u32 placedOp = minitl::min(maxOps, deltas[i].added-opCount);
                    current->m_used += placedOp * operationAddSize;
                    opCount += placedOp;
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
                    u32 maxOps = (bufferSpace - current->m_used) / operationRemoveSize;
                    u32 placedOp = minitl::min(maxOps, deltas[i].removed-opCount);
                    current->m_used += placedOp * operationRemoveSize;
                    opCount += placedOp;
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

    const u32 dummyOffset = m_buckets.end() - m_buckets.begin();
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

void ComponentGroup::repack(weak<EntityStorage> storage,
                            u32 componentIndex,
                            Bucket& bucket, u32 entityCount,
                            OperationDelta operations,
                            i32 offset) const
{
    u32 componentOffset = sizeof(EntityOperation);
    for (u32 i = 0, mask = bucket.acceptMask;
         i < componentIndex;
         ++i, mask >>= 1)
    {
        if (mask & 1)
        {
            componentOffset += m_components[i].size;
        }
    }
    const u32 absoluteComponentIndex = componentIndex + firstComponent;
    EntityStorage::ComponentStorage& componentStorage = *storage->m_components[absoluteComponentIndex];
    const u32 componentSize = m_components[componentIndex].size;
    const u32 firstEntity = bucket.firstEntity;
    const u32 newFirstEntity = bucket.firstEntity + offset;
    const u32 newEntityCount = entityCount + operations.added - operations.removed;
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
        const EntityOperationRemove* lastRemoveOp =  reinterpret_cast<const EntityOperationRemove*>(lastRemoveOpBuffer->m_data + lastRemoveOpOffset);
        for (u32 entityIndex = firstEntity + entityCount;
             entityIndex > firstEntity + newEntityCount
          && firstRemoveOp != lastRemoveOp;
             --entityIndex)
        {
            if (entityIndex-1 == lastRemoveOp->componentIndex)
            {
                if (lastRemoveOpOffset > 0)
                {
                    lastRemoveOpOffset -= sizeof(EntityOperationRemove);
                }
                else
                {
                    lastRemoveOpBuffer = lastRemoveOpBuffer->m_prev;
                    lastRemoveOpOffset = lastRemoveOpBuffer->m_used - sizeof(EntityOperationRemove);
                }
                lastRemoveOp =  reinterpret_cast<const EntityOperationRemove*>(lastRemoveOpBuffer->m_data + lastRemoveOpOffset);
                --removeOperationCount;
            }
            else
            {
                u32 pageSrc = (firstEntity + entityIndex - 1) / componentStorage.elementsPerPage;
                u32 offsetSrc = (firstEntity + entityIndex - 1) % componentStorage.elementsPerPage;
                u32 pageDst = (firstEntity + firstRemoveOp->componentIndex) / componentStorage.elementsPerPage;
                u32 offsetDst = (firstEntity + firstRemoveOp->componentIndex) % componentStorage.elementsPerPage;

                memcpy(componentStorage.pages[pageDst] + offsetDst*componentSize,
                       componentStorage.pages[pageSrc] + offsetSrc*componentSize,
                       componentSize);

                bufferRemoveOffset += sizeof(EntityOperationRemove);
                if (bufferRemoveOffset >= bufferRemove->m_used)
                {
                    bufferRemoveOffset = 0;
                    bufferRemove = bufferRemove->m_next;
                }
                ++currentRemoveOperationFront;
                firstRemoveOp = reinterpret_cast<const EntityOperationRemove*>(bufferRemove->m_data + bufferRemoveOffset);
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
    if (offset < 0 || (offset > 0 && offset >= entityCount))
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
        be_assert (remove->componentIndex < entitiesToMove,
                   "mismatch added/removed ops");
        buffer = bufferAdd->m_data + bufferAddOffset;
        const EntityOperation* add = reinterpret_cast<const EntityOperation*>(buffer);

        u32 pageDst = (newFirstEntity + remove->componentIndex) / componentStorage.elementsPerPage;
        u32 offsetDst = (newFirstEntity + remove->componentIndex) % componentStorage.elementsPerPage;

        memcpy(componentStorage.pages[pageDst] + offsetDst*componentSize,
               reinterpret_cast<const byte*>(add) + componentOffset,
               componentSize);
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

    /* add remaining entities */
    if (operations.added > removeOperationCount)
    {
        for (u32 i = 0; i < operations.added - consumedOps; ++i)
        {
            u32 destIndex = entitiesToMove + i;
            byte* buffer = bufferAdd->m_data + bufferAddOffset;
            const EntityOperation* add = reinterpret_cast<const EntityOperation*>(buffer);
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
            info.buckets[absoluteComponentIndex].bucket = be_checked_numcast<u16>(&bucket - m_buckets.begin());
            info.buckets[absoluteComponentIndex].offset = newFirstEntity + destIndex;
        }
    }
}

void ComponentGroup::repackIndices(weak<EntityStorage> storage,
                                   Bucket& bucket, u32 entityCount,
                                   OperationDelta operations,
                                   i32 offset) const
{
    be_forceuse(operations);
    be_forceuse(storage);
    be_forceuse(entityCount);
#if 0
    EntityStorage::ComponentStorage& componentStorage = storage->m_componentBackLinks[];
    const u32 componentSize = sizeof(u32);
    const u32 firstEntity = bucket.firstEntity;
    const u32 newFirstEntity = bucket.firstEntity + offset;
    const u32 newEntityCount = entityCount + operations.added - operations.removed;
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
        const EntityOperationRemove* lastRemoveOp =  reinterpret_cast<const EntityOperationRemove*>(lastRemoveOpBuffer->m_data + lastRemoveOpOffset);
        for (u32 entityIndex = firstEntity + entityCount;
             entityIndex > firstEntity + newEntityCount;
             --entityIndex)
        {
            if (entityIndex-1 == lastRemoveOp->componentIndex)
            {
                if (lastRemoveOpOffset > 0)
                {
                    lastRemoveOpOffset -= sizeof(EntityOperationRemove);
                }
                else
                {
                    lastRemoveOpBuffer = lastRemoveOpBuffer->m_prev;
                    lastRemoveOpOffset = lastRemoveOpBuffer->m_used - sizeof(EntityOperationRemove);
                }
                lastRemoveOp =  reinterpret_cast<const EntityOperationRemove*>(lastRemoveOpBuffer->m_data + lastRemoveOpOffset);
                --removeOperationCount;
            }
            else
            {
                u32 pageSrc = (firstEntity + entityIndex - 1) / componentStorage.elementsPerPage;
                u32 offsetSrc = (firstEntity + entityIndex - 1) % componentStorage.elementsPerPage;
                u32 pageDst = (firstEntity + firstRemoveOp->componentIndex) / componentStorage.elementsPerPage;
                u32 offsetDst = (firstEntity + firstRemoveOp->componentIndex) % componentStorage.elementsPerPage;

                memcpy(componentStorage.pages[pageDst] + offsetDst*componentSize,
                       componentStorage.pages[pageSrc] + offsetSrc*componentSize,
                       componentSize);

                bufferRemoveOffset += sizeof(EntityOperationRemove);
                if (bufferRemoveOffset >= bufferRemove->m_used)
                {
                    bufferRemoveOffset = 0;
                    bufferRemove = bufferRemove->m_next;
                }
                ++currentRemoveOperationFront;
                firstRemoveOp = reinterpret_cast<const EntityOperationRemove*>(bufferRemove->m_data + bufferRemoveOffset);
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
    if (offset < 0 || offset >= entityCount)
    {
        while (pageSrcStart != pageSrcEnd || countSrcStart != countSrcEnd)
        {
            memcpy(componentStorage.pages[pageDstStart] + countDstStart*componentSize,
                   componentStorage.pages[pageSrcStart] + countSrcStart*componentSize,
                   componentSize);

            if (countSrcStart < componentStorage.elementCount-1)
            {
                ++countSrcStart;
            }
            else
            {
                countSrcStart = 0;
                ++pageSrcStart;
            }

            if (countDstStart < componentStorage.elementCount-1)
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
                countSrcEnd = componentStorage.elementCount-1;
                --pageSrcEnd;
            }

            if (countDstEnd > 0)
            {
                --countDstEnd;
            }
            else
            {
                countDstEnd = componentStorage.elementCount-1;
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
        be_assert (remove->componentIndex < entitiesToMove,
                   "mismatch added/removed ops");
        buffer = bufferAdd->m_data + bufferAddOffset;
        const EntityOperation* add = reinterpret_cast<const EntityOperation*>(buffer);

        u32 pageDst = (newFirstEntity + remove->componentIndex) / componentStorage.elementsPerPage;
        u32 offsetDst = (newFirstEntity + remove->componentIndex) % componentStorage.elementsPerPage;

        memcpy(componentStorage.pages[pageDst] + offsetDst*componentSize,
               reinterpret_cast<const byte*>(add) + componentOffset,
               componentSize);
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

    /* add remaining entities */
    if (operations.added > removeOperationCount)
    {
        for (u32 i = 0; i < operations.added - consumedOps; ++i)
        {
            u32 destIndex = entitiesToMove + i;
            byte* buffer = bufferAdd->m_data + bufferAddOffset;
            const EntityOperation* add = reinterpret_cast<const EntityOperation*>(buffer);
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
            info.buckets[absoluteComponentIndex].bucket = be_checked_numcast<u16>(&bucket - m_buckets.begin());
            info.buckets[absoluteComponentIndex].offset = newFirstEntity + destIndex;
        }
    }
#endif
    bucket.firstEntity = bucket.firstEntity + offset;
}

void ComponentGroup::executeEntityOperations(weak<EntityStorage> storage,
                                             const OperationDelta deltas[])
{
    minitl::Allocator::Block<i32> offsets(Arena::stack(), m_buckets.size()-1);
    for (u32 componentIndex = 0; componentIndex < m_components.size(); ++componentIndex)
    {
        u32 absoluteComponentIndex = componentIndex + firstComponent;
        EntityStorage::ComponentStorage& componentStorage = *storage->m_components[absoluteComponentIndex];
        const u32 mask = 1 << componentIndex;
        memset(offsets.begin(), 0, offsets.byteCount());
        i32 absoluteOffset = 0;
        for (u32 bucket = 0; bucket < m_buckets.size()-1; ++bucket)
        {
            offsets[bucket] = absoluteOffset;
            if (m_buckets[bucket].acceptMask & mask)
            {
                absoluteOffset += deltas[bucket].added - deltas[bucket].removed;
            }
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
                                  || offsets[i] >= 0;
                    bool spaceRight = i == m_buckets.size()-2
                                   || bucketsProcessed[i+1]
                                   || offsets[i+1] <= 0;
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
                                  || offsets[i-1] >= 0;
                    bool spaceRight = i == m_buckets.size()-1
                                   || bucketsProcessed[i]
                                   || offsets[i] <= 0;
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
                u32 entityCount = m_buckets[bucketToProcess+1].firstEntity
                                - m_buckets[bucketToProcess].firstEntity;
                repack(storage, componentIndex, m_buckets[bucketToProcess], entityCount,
                       deltas[bucketToProcess], offsets[bucketToProcess]);
            }
        }
        if (absoluteOffset < 0)
        {
            componentStorage.shrink(absoluteOffset);
        }
    }

    {
        i32 absoluteOffset = 0;
        for (u32 bucket = 0; bucket < m_buckets.size()-1; ++bucket)
        {
            offsets[bucket] = absoluteOffset;
            absoluteOffset += deltas[bucket].added - deltas[bucket].removed;
        }
        if (absoluteOffset > 0)
        {
            //componentStorage.reserve(absoluteOffset);
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
                                  || offsets[i] >= 0;
                    bool spaceRight = i == m_buckets.size()-2
                                   || bucketsProcessed[i+1]
                                   || offsets[i+1] <= 0;
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
                                  || offsets[i-1] >= 0;
                    bool spaceRight = i == m_buckets.size() - 1
                                   || bucketsProcessed[i]
                                   || offsets[i] <= 0;
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
            u32 entityCount = m_buckets[bucketToProcess+1].firstEntity
                            - m_buckets[bucketToProcess].firstEntity;
            repackIndices(storage, m_buckets[bucketToProcess], entityCount,
                          deltas[bucketToProcess], offsets[bucketToProcess]);
        }
        m_buckets[m_buckets.size()-1].firstEntity += absoluteOffset;
        if (absoluteOffset < 0)
        {
            //componentStorage.shrink(absoluteOffset);
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
    byte* componentBuffer = operationBuffer + sizeof(EntityOperation);
    EntityOperation* operation = reinterpret_cast<EntityOperation*>(operationBuffer);
    operation->owner = e.id;
    operation->maskBefore = originalMask;
    operation->maskAfter = originalMask | (1 << componentIndex);
    operation->size = static_cast<u32>(sizeof(EntityOperation) + info.size);
    memcpy(componentBuffer, &c, info.size);
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
