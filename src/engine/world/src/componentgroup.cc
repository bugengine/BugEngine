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

struct EntityOperation
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

}

struct ComponentGroup::OperationDelta
{
    u32 added;
    u32 removed;
    byte* pageAdd;
    byte* pageRemove;
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
    ,   m_componentCounts((u32*)Arena::game().alloc(
                              sizeof(u32)*componentTypes.size()*bucketMasks.size()))
    ,   m_entityOperation(new (m_allocator.allocate()) OperationBuffer)
    ,   firstComponent(firstComponent)
    ,   lastComponent(firstComponent + (u32)componentTypes.size())
{
    u32 index = 0;
    u32 count = (u32)componentTypes.size();
    memset(m_componentCounts, 0, sizeof(u32)*count*bucketMasks.size());
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
        m_buckets[index] = Bucket(m_componentCounts + count*index, *it, size);
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
    m_entityOperation = new (m_allocator.allocate()) OperationBuffer;
    minitl::Allocator::Block<byte> componentBuffer(Arena::stack(), m_componentsTotalSize);
    minitl::Allocator::Block<u32> componentOffsets(Arena::stack(), m_components.size());
    for (u32 o = 0, i = 0; i < m_components.size(); o += m_components[i].size, ++i)
    {
        componentOffsets[i] = o;
    }
    for (OperationBuffer* b = oldBuffer; b; /* nothing */)
    {
        EntityOperation* last = (EntityOperation*)(b->m_data + b->m_used);
        for (EntityOperation* operation = (EntityOperation*)b->m_data;
             operation < last;
             operation = operation->next())
        {
            if (operation->owner != s_invalidEntity)
            {
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
                for (OperationBuffer* b = m_entityOperation; b; b = b->m_next)
                {
                    EntityOperation* last = (EntityOperation*)(b->m_data + b->m_used);
                    for (EntityOperation* sibling = (EntityOperation*)b->m_data;
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
                if (buckets.first != buckets.second)
                {
                    copyComponents(storage, operation->owner, componentBuffer.begin(),
                                   maskBefore & maskAfter & ~maskChanged, componentOffsets.begin());

                    EntityOperation* removeOperation = new (allocOperation(0)) EntityOperation;
                    removeOperation->owner = operation->owner;
                    removeOperation->maskBefore = buckets.first->acceptMask;
                    removeOperation->maskAfter = 0;
                    removeOperation->size = sizeof(EntityOperation);
                    deltas[buckets.first-m_buckets.begin()].removed++;

                    u32 extraSize = buckets.second->componentSize;
                    EntityOperation* addOperation = new (allocOperation(extraSize)) EntityOperation;
                    addOperation->owner = operation->owner;
                    addOperation->maskBefore = 0;
                    addOperation->maskAfter = buckets.second->acceptMask;
                    addOperation->size = sizeof(EntityOperation) + extraSize;
                    deltas[buckets.second-m_buckets.begin()].added++;
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
                            EntityOperation* removeOperation = new (allocOperation(0)) EntityOperation;
                            removeOperation->owner = operation->owner;
                            removeOperation->maskBefore = buckets.first->acceptMask;
                            removeOperation->maskAfter = 0;
                            removeOperation->size = sizeof(EntityOperation);
                            deltas[componentBucket-m_buckets.begin()].removed++;
                        }
                        if (remainderMaskAdd & 1)
                        {
                            be_assert(componentBucket->acceptMask == (1<<i),
                                      "invalid component bucket for component %d" | i);
                            EntityOperation* addOperation = new (allocOperation(0)) EntityOperation;
                            addOperation->owner = operation->owner;
                            addOperation->maskBefore = 0;
                            addOperation->maskAfter = buckets.second->acceptMask;
                            addOperation->size = sizeof(EntityOperation) + m_components[i].size;
                            packComponents(addOperation->maskAfter, componentBuffer.begin(),
                                           (byte*)addOperation + sizeof(EntityOperation),
                                           componentOffsets);
                            deltas[componentBucket-m_buckets.begin()].added++;
                        }
                    }
                }
            }
        }
        OperationBuffer* next = b->m_next;
        b->~OperationBuffer();
        m_allocator.free(b);
        b = next;
    }
    be_forceuse(deltas);
}

void ComponentGroup::sortEntityOperations(const OperationDelta deltas[])
{
    be_forceuse(deltas);
}

void ComponentGroup::runEntityOperations(weak<EntityStorage> storage)
{
    be_forceuse(storage);
    {
        minitl::Allocator::Block<OperationDelta> deltas(Arena::stack(), m_buckets.size());
        for (u32 i = 0; i < m_buckets.size(); ++i)
        {
            deltas[i].added = 0;
            deltas[i].removed = 0;
            deltas[i].pageAdd = 0;
            deltas[i].pageRemove = 0;
            deltas[i].pageAddOffset = 0;
            deltas[i].pageRemoveOffset = 0;
        }
        groupEntityOperations(storage, deltas.begin());
        sortEntityOperations(deltas.begin());
    }
    OperationBuffer* buffer = m_entityOperation->m_next;
    while (buffer)
    {
        OperationBuffer* dealloc = buffer;
        buffer = buffer->m_next;
        dealloc->~OperationBuffer();
        m_allocator.free(buffer);
    }
    m_entityOperation->m_next = 0;
    m_entityOperation->m_used = 0;
}

void ComponentGroup::freeBuffers()
{
    Arena::game().free(m_componentCounts);
    be_assert(m_entityOperation->m_used == 0 && m_entityOperation->m_next == 0,
              "Entity operation still in use when ComponentGroup is destroyed");
    m_entityOperation->~OperationBuffer();
    m_allocator.free(m_entityOperation);
}

void ComponentGroup::addComponent(Entity e, u32 originalMask, const Component& c, u32 componentIndex)
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
        OperationBuffer* buffer = m_entityOperation;
        u32 offset = buffer->m_offset.addExchange(totalSize);
        if (offset + totalSize + sizeof(OperationBuffer) > 256) //m_allocator.blockSize())
        {
            OperationBuffer* newBuffer = new (m_allocator.allocate()) OperationBuffer;
            newBuffer->m_next = buffer;
            if (m_entityOperation.setConditional(newBuffer, buffer) != buffer)
            {
                newBuffer->~OperationBuffer();
                m_allocator.free(newBuffer);
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
