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

static raw<const RTTI::Method::Overload> getMethodFromClass(raw<const RTTI::Class> type, istring name)
{
    raw<const RTTI::Method> method = type->getMethod(name);
    be_assert_recover(method,
                      "could not locate method \"%s\" for component %s" | name | type->fullname(),
                      return raw<const RTTI::Method::Overload>());
    for (raw<const RTTI::Method::Overload> overload = method->overloads; overload; overload = overload->next)
    {
        RTTI::Type t = RTTI::Type::makeType(type, RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
        if (overload->parameterCount == 1 && t.isA(overload->params->type))
        {
            return overload;
        }
    }
    return raw<const RTTI::Method::Overload>();
}

struct ComponentOffset
{
private:
    u32 m_buffer;
public:
    ComponentOffset()
        :   m_buffer(0xFFFFFFFF)
    {
    }
    ComponentOffset(u32 bucket, u32 offset)
    {
        be_assert(bucket < 0xFF, "bucket out of range");
        be_assert(offset < 0xFFFFFF, "offset out of range");
        m_buffer = (bucket << 24) | (offset & 0xFFFFFF);
    }

    u32 offset() const { return m_buffer & 0xFFFFFF; }
    u32 bucket() const { return m_buffer >> 24; }
};

static inline u32 getFirstBit(u32 mask)
{
    u32 result = 0;
    for (u32 m = mask; m; m <<= 1, result++)
    {
        if (m & 0x1)
            break;
    }
    return result;
}

struct EntityOperationBase
{
    u32 entityId;
    u32 size;
    u32 originalMask;
    u32 componentMaskAdd;
    u32 componentMaskRemove;
    u32 componentStorage;
    u16 bucketOrigin;
    u16 bucketDestination;
    u32 componentIndex;
};

struct EntityOperation : public EntityOperationBase
{
    u8 componentBuffer[4];
};

}

struct ComponentGroup::Delta
{
    i32 absoluteOffset;
    u16 added;
    u16 removed;
};

struct ComponentGroup::Offset
{
    u32 offsetRemoved;
    u32 offsetAdded;
};

struct EntitySort
{
    bool operator()(const EntityOperationBase& first, const EntityOperationBase& second)
    {
        return first.componentIndex < second.componentIndex;
    }
};

Bucket::Bucket()
    :   componentCounts(0)
    ,   acceptMask(0)
    ,   maskSize(0)
    ,   firstComponent(0)
{
}

Bucket::Bucket(u32* componentCounts, u32 acceptMask)
    :   componentCounts(componentCounts)
    ,   acceptMask(acceptMask)
    ,   maskSize(bitCount(acceptMask))
    ,   firstComponent(getFirstBit(acceptMask))
{
}


ComponentGroup::ComponentGroup(u32 firstComponent, const minitl::vector< raw<const RTTI::Class> >& componentTypes,
                               const minitl::vector<u32>& bucketMasks, SystemAllocator& allocator)
    :   m_operationAllocator(allocator)
    ,   m_buckets(Arena::game(), (u32)bucketMasks.size())
    ,   m_components(Arena::game(), (u32)componentTypes.size())
    ,   m_componentsTotalSize(0)
    ,   m_componentCounts((u32*)Arena::game().alloc(sizeof(u32)*componentTypes.size()*bucketMasks.size()))
    ,   m_componentOperations((u8*)m_operationAllocator.allocate())
    ,   m_operationOffset(i_u32::Zero)
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
        m_buckets[index] = Bucket(m_componentCounts + count*index, *it);
    }
    minitl::vector< raw<const RTTI::Class> >::const_iterator begin = componentTypes.begin();
    for (minitl::vector< raw<const RTTI::Class> >::const_iterator it = begin;
        it != componentTypes.end();
        ++it)
    {
        ComponentInfo& info = m_components[be_checked_numcast<u32>(minitl::distance(begin, it))];
        info.componentType = *it;
        info.size = (*it)->size;
        info.created = getMethodFromClass(*it, "created");
        if (!info.created)
        {
            be_error("component type %s: invalid signature for method \"created\"" | (*it)->fullname());
        }
        info.destroyed = getMethodFromClass(*it, "destroyed");
        if (!info.destroyed)
        {
            be_error("component type %s: invalid signature for method \"destroyed\"" | (*it)->fullname());
        }

        m_componentsTotalSize += (*it)->size;
    }
}

ComponentGroup::~ComponentGroup()
{
}

ComponentGroup::BucketPair ComponentGroup::findBuckets(u32 mask1, u32 mask2)
{
    BucketPair result;
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

void ComponentGroup::mergeEntityOperation(u8* __restrict source, const u8* __restrict merge)
{
    EntityOperation* result = reinterpret_cast<EntityOperation*>(source);
    const EntityOperation* with = reinterpret_cast<const EntityOperation*>(merge);

    u8* buffer = result->componentBuffer;
    u32 bufferSize = result->size - (u32)sizeof(EntityOperationBase);
    const u8* mergedBuffer = with->componentBuffer;
    for (u32 i = 0; i < lastComponent-firstComponent; ++i)
    {
        u32 m = 1 << i;
        if (result->componentMaskAdd & m)
        {
            be_assert((with->componentMaskAdd & m) == 0,
                      "Entity %d: adding component %s twice in a frame"
                        | result->entityId | m_components[i].componentType->fullname());
            u32 size = m_components[i].size;
            if (with->componentMaskRemove & m)
            {
                result->size -= size;
                bufferSize -= size;
                memmove(buffer, buffer + size, bufferSize);
                result->componentMaskAdd &= ~m;
            }
            else
            {
                buffer += size;
                bufferSize -= size;
            }
        }
        else if (with->componentMaskAdd & m)
        {
            result->componentMaskAdd |= m;
            u32 sizeToMove = m_components[i].size;
            if (bufferSize)
            {
                memmove(buffer + sizeToMove, buffer, bufferSize);
            }
            memcpy(buffer, mergedBuffer, sizeToMove);
            mergedBuffer += sizeToMove;
            buffer += sizeToMove;
            result->size += sizeToMove;
        }
        else if (with->componentMaskRemove & m)
        {
            be_assert((result->componentMaskRemove & m) == 0,
                      "Entity %d: adding component %s twice in a frame"
                        | result->entityId | m_components[i].componentType->fullname());
            result->componentMaskRemove |= m;
        }
    }
}

void ComponentGroup::moveBucketComponents(u32 componentIndex,
                                          Bucket* __restrict bucket,
                                          u8* __restrict operationsRemove,
                                          u8* __restrict operationsAdd,
                                          u8* __restrict operationsEnd,
                                          const u8* __restrict componentBuffer,
                                          i32 delta)
{
    be_forceuse(componentIndex);
    be_forceuse(bucket);
    be_forceuse(operationsEnd);
    be_forceuse(componentBuffer);

    u8* operationsRemoveBuffer = operationsRemove;
    u8* operationsAddBuffer = operationsAdd;
    //u32 originalDelta = delta;
    // add components at beginning of delta
    while(operationsAddBuffer < operationsEnd && delta < 0)
    {
        EntityOperation* operationAdd = (EntityOperation*)operationsAddBuffer;
        operationsAddBuffer += operationAdd->size;
        delta ++;
    }

    // move components into delta

    // move added components in place of removed ones
    while(operationsRemoveBuffer < operationsAdd && operationsAddBuffer < operationsEnd)
    {
        EntityOperation* operationRemove = (EntityOperation*)operationsRemoveBuffer;
        EntityOperation* operationAdd = (EntityOperation*)operationsAddBuffer;
        operationsRemoveBuffer += operationRemove->size;
        operationsAddBuffer += operationAdd->size;
    }

    // remove remaining components
    if (operationsRemoveBuffer != operationsAdd)
    {
        minitl::sort((EntityOperationBase*)operationsRemoveBuffer,
                     (EntityOperationBase*)operationsAdd,
                     EntitySort());
        /* move entities around */
    }

    // add remaining components
    if (operationsAddBuffer != operationsEnd)
    {
        /* there should be place at the end of the buffer, place them there */
    }
}

void ComponentGroup::moveComponents(u32 componentIndex, Bucket* begin, Bucket* end,
                                                   u8* __restrict operations,
                                                   Offset* __restrict operationOffsetPerBucket,
                                                   Delta* __restrict deltas,
                                                   const u8* __restrict componentBuffer)
{
    be_info("moving buckets %d - %d" | (begin - m_buckets.begin()) | (end - m_buckets.begin()));
    u32 componentCount = m_components.size();
    Bucket* lowerBound = 0;
    Bucket* upperBound = 0;
    for (lowerBound = begin; lowerBound < end; ++lowerBound)
    {
        minitl::size_type bucketIndex = minitl::distance(begin, lowerBound);
        const Delta& delta = deltas[bucketIndex*componentCount + componentIndex];
        if (delta.added + delta.absoluteOffset > delta.removed)
        {
            break;
        }
        Offset operationOffset = operationOffsetPerBucket[bucketIndex];
        Offset operationOffsetEnd = operationOffsetPerBucket[bucketIndex+1];
        u8* operationRemoveBuffer = operations + operationOffset.offsetRemoved;
        u8* operationAddBuffer = operations + operationOffset.offsetAdded;
        u8* operationEndBuffer = operations + operationOffsetEnd.offsetRemoved;
        moveBucketComponents(componentIndex, lowerBound, operationRemoveBuffer,
                             operationAddBuffer, operationEndBuffer, componentBuffer,
                             delta.absoluteOffset);
    }
    for (upperBound = end-1; upperBound > lowerBound; --upperBound)
    {
        minitl::size_type bucketIndex = minitl::distance(begin, upperBound);
        const Delta& delta = deltas[bucketIndex*componentCount + componentIndex];
        if (delta.absoluteOffset < 0)
        {
            break;
        }
        Offset operationOffset = operationOffsetPerBucket[bucketIndex];
        Offset operationOffsetEnd = operationOffsetPerBucket[bucketIndex+1];
        u8* operationRemoveBuffer = operations + operationOffset.offsetRemoved;
        u8* operationAddBuffer = operations + operationOffset.offsetAdded;
        u8* operationEndBuffer = operations + operationOffsetEnd.offsetRemoved;
        moveBucketComponents(componentIndex, lowerBound, operationRemoveBuffer,
                             operationAddBuffer, operationEndBuffer, componentBuffer,
                             delta.absoluteOffset);
    }
    if (lowerBound == upperBound)
    {
        minitl::size_type bucketIndex = minitl::distance(begin, upperBound);
        const Delta& delta = deltas[bucketIndex*componentCount + componentIndex];
        Offset operationOffset = operationOffsetPerBucket[bucketIndex];
        Offset operationOffsetEnd = operationOffsetPerBucket[bucketIndex+1];
        u8* operationRemoveBuffer = operations + operationOffset.offsetRemoved;
        u8* operationAddBuffer = operations + operationOffset.offsetAdded;
        u8* operationEndBuffer = operations + operationOffsetEnd.offsetRemoved;
        moveBucketComponents(componentIndex, lowerBound, operationRemoveBuffer,
                             operationAddBuffer, operationEndBuffer, componentBuffer,
                             delta.absoluteOffset);
    }
    else
    {
        for (Bucket* b = lowerBound+1; b < upperBound; ++b)
        {
            minitl::size_type bucketIndex = minitl::distance(begin, b);
            const Delta& delta = deltas[bucketIndex*componentCount + componentIndex];

            if (delta.added + delta.absoluteOffset <= delta.removed)
            {
                moveComponents(componentIndex, lowerBound, b, operations,
                               &operationOffsetPerBucket[lowerBound - begin],
                               &deltas[(lowerBound - begin) * componentCount],
                               componentBuffer);
                moveComponents(componentIndex, b + 1, upperBound + 1, operations,
                               &operationOffsetPerBucket[b + 1 - begin],
                               &deltas[(b + 1 - begin) * componentCount],
                               componentBuffer);
                break;
            }
        }
    }
}

void ComponentGroup::runEntityOperations(weak<EntityStorage> storage,
                                         u8* __restrict buffer,
                                         u8* __restrict componentBuffer)
{
    const u32 componentCount = m_components.size();
    Delta* deltas = (Delta*)malloca(sizeof(Delta) * m_buckets.size() * componentCount);
    memset(deltas, 0, sizeof(Delta) * m_buckets.size() * componentCount);
    u32 scratchBufferSize = sizeof(Offset) * (m_buckets.size() + 1) + sizeof(u32);
    u8* scratchBuffer = (u8*)malloca(scratchBufferSize);
    memset(scratchBuffer, 0, scratchBufferSize);
    Offset* operationOffsetsPerBucket = (Offset*)scratchBuffer;
    Offset* operationSizePerBucket = (Offset*)(scratchBuffer + sizeof(u32));
    u8* destination = buffer;
    u8* destinationEnd = destination;

    u8* source = m_componentOperations;
    u8* end = source + m_operationOffset;
    u8* componentBackup = componentBuffer;

    EntityOperation* result = reinterpret_cast<EntityOperation*>(destinationEnd);
    for (u8* current = source; current < end; /*nothing*/)
    {
        const EntityOperation* operation = reinterpret_cast<const EntityOperation*>(current);
        current += operation->size;
        if (operation->entityId != 0xFFFFFFFF)
        {
            memcpy(result, operation, operation->size);
            for (u8* lookup = current; lookup != end; /*nothing*/)
            {
                EntityOperation* check = reinterpret_cast<EntityOperation*>(lookup);
                lookup += check->size;
                if (check->entityId == operation->entityId)
                {
                    check->entityId = 0xFFFFFFFF;
                    mergeEntityOperation((u8*)result, (u8*)check);
                }
            }

            u32 maskBefore = result->originalMask;
            u32 maskAfter = (maskBefore | result->componentMaskAdd) & ~result->componentMaskRemove;
            BucketPair insertion = findBuckets(maskBefore, maskAfter);
            u32 remainderAdd = maskAfter & ~insertion.second->acceptMask;
            u32 remainderRemoved = result->componentMaskRemove & ~insertion.first->acceptMask;
            result->bucketOrigin = be_checked_numcast<u16>(insertion.first - m_buckets.begin());
            result->bucketDestination = be_checked_numcast<u16>(insertion.second - m_buckets.begin());
            result->componentStorage = be_checked_numcast<u32>(componentBackup - componentBuffer);
            be_assert(componentBackup + m_componentsTotalSize < componentBuffer + storage->m_operationAllocator->blockSize(),
                      "ran out of component backup space");
            Entity e = {result->entityId};
            const EntityInfo& info = storage->getEntityInfo(e);
            componentBackup += storage->store(info, componentBackup, firstComponent,
                                              insertion.first->acceptMask & maskAfter);
            u32 indexSourceBucket = be_checked_numcast<u32>(insertion.first - m_buckets.begin());
            u32 indexDestinationBucket = be_checked_numcast<u32>(insertion.second - m_buckets.begin());
            if (insertion.first != insertion.second)
            {
                for (u32 i = 0; i < componentCount; ++i)
                {
                    if ((insertion.first->acceptMask >> i) & 0x1)
                    {
                        deltas[componentCount*(insertion.first - m_buckets.begin()) + i].removed ++;
                    }
                    if ((insertion.second->acceptMask >> i) & 0x1)
                    {
                        deltas[componentCount*(insertion.second - m_buckets.begin()) + i].added ++;
                    }
                }

                if (insertion.first->acceptMask != 0)
                {
                    u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperationBase));
                    operationSizePerBucket[indexSourceBucket].offsetRemoved += operationSize;
                }
                if (insertion.second->acceptMask != 0)
                {
                    operationSizePerBucket[indexDestinationBucket].offsetAdded += result->size;
                }
            }
            if (remainderAdd)
            {
                u32 componentIndex = 0;
                for (Bucket* bucket = m_buckets.end() - componentCount - 1;
                     bucket != m_buckets.end() - 1;
                     ++bucket, ++componentIndex)
                {
                    be_assert(bucket->maskSize == 1, "invalid bucket");
                    u32 bucketIndex = be_checked_numcast<u32>(bucket - m_buckets.begin());
                    if (bucket->acceptMask & remainderAdd)
                    {
                        deltas[bucketIndex*componentCount + componentIndex].added ++;
                        u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperationBase));
                        operationSize += m_components[componentIndex].size;
                        operationSizePerBucket[bucketIndex].offsetAdded += operationSize;
                    }
                }
            }
            for (u32 componentIndex = 0, bucketIndex = m_buckets.size() - 1 - componentCount;
                 remainderRemoved != 0;
                 remainderRemoved >>=1, componentIndex++, bucketIndex++)
            {
                if (remainderRemoved & 0x1)
                {
                    deltas[bucketIndex*componentCount + componentIndex].added ++;
                    u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperationBase));
                    operationSizePerBucket[bucketIndex].offsetRemoved += operationSize;
                }
            }

            destinationEnd += result->size;
            result = reinterpret_cast<EntityOperation*>(destinationEnd);
        }
    }

    /* sort operations per bucket */
    destination = m_componentOperations;

    {
        u32 offset = operationSizePerBucket[0].offsetRemoved;
        u32 size = operationSizePerBucket[0].offsetAdded;
        operationSizePerBucket[0].offsetRemoved = 0;
        operationSizePerBucket[0].offsetAdded = offset;
        offset += size;
        for (u32 i = 1; i < m_buckets.size(); ++i)
        {
            size = operationSizePerBucket[i].offsetRemoved;
            operationSizePerBucket[i].offsetRemoved = offset;
            offset += size;
            size = operationSizePerBucket[i].offsetAdded;
            operationSizePerBucket[i].offsetAdded = offset;
            offset += size;
            u32 index = i*componentCount;
            for (u32 c = 0; c < componentCount; ++c)
            {
                deltas[index + c].absoluteOffset = deltas[(i - 1)*componentCount + c].absoluteOffset
                                                 + deltas[(i - 1)*componentCount + c].added
                                                 - deltas[(i - 1)*componentCount + c].removed;
            }
        }
    }

    for (u8* current = buffer; current < destinationEnd; /*nothing*/)
    {
        const EntityOperation* operation = reinterpret_cast<const EntityOperation*>(current);
        if (operation->bucketOrigin != operation->bucketDestination)
        {
            Bucket& bucketOrigin = m_buckets[operation->bucketOrigin];
            Bucket& bucketDestination = m_buckets[operation->bucketDestination];

            if (bucketOrigin.acceptMask != 0)
            {
                u32 sizeRemove = be_checked_numcast<u32>(sizeof(EntityOperationBase));
                u32& offset = operationSizePerBucket[operation->bucketOrigin].offsetRemoved;
                memcpy(destination + offset, current, sizeRemove);

                EntityOperation* result = reinterpret_cast<EntityOperation*>(destination + offset);
                result->componentMaskAdd = 0;
                result->componentMaskRemove = bucketOrigin.acceptMask;
                result->size = sizeRemove;
                Entity e = { result->entityId };
                result->componentIndex = storage->getComponentIndex(e, *this, bucketOrigin);

                offset += sizeRemove;
            }

            if (bucketDestination.acceptMask != 0)
            {
                u32& offset = operationSizePerBucket[operation->bucketDestination].offsetAdded;
                memcpy(destination + offset, current, operation->size);

                EntityOperation* result = reinterpret_cast<EntityOperation*>(destination + offset);
                result->componentMaskAdd = bucketDestination.acceptMask;
                result->componentMaskRemove = 0;
                result->size = operation->size;
                offset += operation->size;
            }
        }
        u32 maskAfter = (operation->originalMask | operation->componentMaskAdd)
                      & ~operation->componentMaskRemove;
        u32 remainderAdd = maskAfter & ~m_buckets[operation->bucketDestination].acceptMask;
        if (remainderAdd)
        {
            u32 component = 0;
            const u8* componentBuffer = operation->componentBuffer;
            for (Bucket* bucket = m_buckets.end() - componentCount - 1;
                 bucket != m_buckets.end() - 1;
                 ++bucket, ++component)
            {
                be_assert(bucket->maskSize == 1, "invalid bucket");
                u32 index = be_checked_numcast<u32>(bucket - m_buckets.begin());
                if (bucket->acceptMask & remainderAdd)
                {
                    u32& offset = operationSizePerBucket[index].offsetAdded;
                    memcpy(destination + offset, current, sizeof(EntityOperationBase));
                    EntityOperation* result = reinterpret_cast<EntityOperation*>(destination + offset);
                    result->componentMaskAdd = bucket->acceptMask;
                    result->componentMaskRemove = 0;
                    u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperationBase))
                                      + m_components[component].size;
                    result->size = operationSize;
                    offset += sizeof(EntityOperationBase);
                    memcpy(destination + offset, componentBuffer, m_components[component].size);
                    offset += m_components[component].size;
                    componentBuffer += m_components[component].size;
                }
                else if (bucket->acceptMask & operation->componentMaskAdd)
                {
                    componentBuffer += m_components[component].size;
                }
            }
        }
        u32 remainderRemoved = operation->componentMaskRemove & ~m_buckets[operation->bucketOrigin].acceptMask;
        if (remainderRemoved)
        {
            u32 offset = 0;
            for (Bucket* bucket = m_buckets.end() - componentCount - 1; bucket != m_buckets.end() - 1; ++bucket, ++offset)
            {
                be_assert(bucket->maskSize == 1, "invalid bucket");
                u32 index = be_checked_numcast<u32>(bucket - m_buckets.begin());
                if (bucket->acceptMask & remainderRemoved)
                {
                    u32& offset = operationSizePerBucket[index].offsetRemoved;
                    memcpy(destination + offset, current, sizeof(EntityOperationBase));
                    EntityOperation* operationRemove = reinterpret_cast<EntityOperation*>(destination + offset);
                    operationRemove->componentMaskAdd = 0;
                    operationRemove->componentMaskRemove = bucket->acceptMask;
                    operationRemove->size = be_checked_numcast<u32>(sizeof(EntityOperationBase));
                    offset += sizeof(EntityOperationBase);
                }
            }
        }
        current = current + operation->size;
    }

    for (u32 componentIndex = 0; componentIndex < m_components.size(); ++componentIndex)
    {
        moveComponents(componentIndex, m_buckets.begin(), m_buckets.end(), destination,
                       operationOffsetsPerBucket, deltas, componentBuffer);
    }
#if 1
    /*i32* d = (i32*)malloca(sizeof(i32) * componentCount);
    memset(d, 0, sizeof(i32) * componentCount);
    for (u32 i = 0; i < buckets.size(); ++i)
    {
        for (u32 c = 0; c < componentCount; ++c)
        {
            d[c] += deltas[i*componentCount + c];
        }
    }
    for (u32 i = 0; i < componentCount; ++i)
    {
        be_info("component delta %s: %d" | components[i].componentType->fullname() | d[i]);
    }
    freea(d);*/
    u32 bucket = 0;
    for (u8* current = destination;
         current < destination + operationSizePerBucket[m_buckets.size()-1].offsetRemoved;
         /*nothing*/)
    {
        while (operationOffsetsPerBucket[bucket].offsetRemoved == current - destination)
        {
            be_info("Bucket %d" | bucket);
            bucket++;
        }
        const EntityOperation* operation = reinterpret_cast<const EntityOperation*>(current);
        current += operation->size;
        be_info(" Entity %d:" | operation->entityId);
        const u8* buffer = operation->componentBuffer;
        for (u32 c = 0; c < lastComponent-firstComponent; ++c)
        {
            u32 m = 1 << c;
            if (operation->componentMaskAdd & m)
            {
                be_info("  added %s[%d]"
                        | m_components[c].componentType->fullname() | *reinterpret_cast<const u32*>(buffer));
                buffer += m_components[c].componentType->size;
            }
            if (operation->componentMaskRemove & m)
            {
                be_info("  removed %s" | m_components[c].componentType->fullname());
            }
        }
    }
#endif

    freea(scratchBuffer);
    freea(deltas);

    m_operationOffset = 0;
}

void ComponentGroup::freeBuffers()
{
    m_operationAllocator.free(m_componentOperations);
    Arena::game().free(m_componentCounts);
}

void ComponentGroup::addComponent(Entity e, u32 originalMask, const Component& c, u32 componentIndex)
{
    u32 componentSize = m_components[componentIndex].size;
    u32 size = be_checked_numcast<u32>(sizeof(EntityOperation)) + componentSize - 4;
    u32 offset = (m_operationOffset += size) - size;
    be_assert_recover(offset < m_operationAllocator.blockSize(),
                      "Entity %d: could not add component of type %s: ran out of buffer"
                        | e.id | m_components[componentIndex].componentType->fullname(),
                      return);
    EntityOperation* buffer = reinterpret_cast<EntityOperation*>(m_componentOperations + offset);
    buffer->entityId = e.id;
    buffer->size = size;
    buffer->originalMask = originalMask;
    buffer->componentMaskAdd = 1 << componentIndex;
    buffer->componentMaskRemove = 0;
    buffer->componentStorage = 0;
    buffer->bucketOrigin = 0;
    buffer->bucketDestination = 0;
    memcpy(buffer->componentBuffer, &c, componentSize);
}

void ComponentGroup::removeComponent(Entity e, u32 originalMask, u32 componentIndex)
{
    u32 size = be_checked_numcast<u32>(sizeof(EntityOperation)) - 4;
    u32 offset = (m_operationOffset += size) - size;
    be_assert_recover(offset < m_operationAllocator.blockSize(),
                      "Entity %d: could not add component of type %s: ran out of buffer"
                        | e.id | m_components[componentIndex].componentType->fullname(),
                      return);
    EntityOperation* buffer = reinterpret_cast<EntityOperation*>(m_componentOperations + offset);
    buffer->entityId = e.id;
    buffer->size = size;
    buffer->originalMask = originalMask;
    buffer->componentMaskAdd = 0;
    buffer->componentMaskRemove = 1 << componentIndex;
    buffer->componentStorage = 0;
    buffer->bucketOrigin = 0;
    buffer->bucketDestination = 0;
}


}}
