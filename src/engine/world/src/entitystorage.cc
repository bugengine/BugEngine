/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <scheduler/task/method.hh>
#include    <minitl/bitset.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>
#include    <minitl/algorithm.hh>

namespace BugEngine { namespace World
{

namespace
{

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

struct EntityOperation
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
    u8 componentBuffer[4];
};

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

struct GroupInfo
{
    typedef minitl::vector< raw<const RTTI::Class> > ComponentVector;
    typedef minitl::vector< minitl::array< raw<const RTTI::Class> > > PartitionVector;
    ComponentVector components;
    PartitionVector partitions;

    GroupInfo(const minitl::array< raw<const RTTI::Class> >& initial)
        :   components(Arena::temporary(), initial.begin(), initial.end())
        ,   partitions(Arena::temporary())
    {
        partitions.push_back(initial);
    }

    template< typename IT >
    bool isCommon(IT firstComponent, IT lastComponent)
    {
        for (IT it =firstComponent; it != lastComponent; ++it)
        {
            for (ComponentVector::const_iterator it2 = components.begin(); it2 != components.end(); ++it2)
            {
                if (*it == *it2)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool tryMerge(const GroupInfo& otherGroup)
    {
        if (isCommon(otherGroup.components.begin(), otherGroup.components.end()))
        {
            for (ComponentVector::const_iterator component = otherGroup.components.begin();
                 component != otherGroup.components.end();
                 ++component)
            {
                if (find(*component, components.begin(), components.end()) == components.end())
                {
                    components.push_back(*component);
                }
            }
            for (PartitionVector::const_iterator partition = otherGroup.partitions.begin();
                 partition != otherGroup.partitions.end();
                  ++partition)
            {
                partitions.push_back(*partition);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
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

}

EntityStorage::ComponentGroup::Bucket::Bucket()
    :   componentCounts(0)
    ,   acceptMask(0)
    ,   maskSize(0)
    ,   firstComponent(0)
{
}

EntityStorage::ComponentGroup::Bucket::Bucket(u32* componentCounts, u32* componentOffsets, u32 acceptMask)
    :   componentCounts(componentCounts)
    ,   componentOffsets(componentOffsets)
    ,   acceptMask(acceptMask)
    ,   maskSize(bitCount(acceptMask))
    ,   firstComponent(getFirstBit(acceptMask))
{
}


EntityStorage::ComponentGroup::ComponentGroup(u32 firstComponent, u32 componentCount, u32 componentsTotalSize,
                                              u32* componentCounts, u32* componentOffsets,
                                              const minitl::vector<u32>& bucketMasks, u8* operationBuffer)
    :   buckets(Arena::game(), (u32)bucketMasks.size())
    ,   components(Arena::game(), componentCount)
    ,   firstComponent(firstComponent)
    ,   lastComponent(firstComponent + componentCount)
    ,   componentsTotalSize(componentsTotalSize)
    ,   componentCounts(componentOffsets)
    ,   componentOffsets(componentCounts)
    ,   componentOperations(operationBuffer)
    ,   operationOffset(i_u32::Zero)
{
    u32 index = 0;
    for (minitl::vector<u32>::const_iterator it = bucketMasks.begin();
         it != bucketMasks.end();
         ++it, ++index)
    {
        buckets[index] = Bucket(componentCounts + componentCount*index, componentOffsets + componentCount*index, *it);
    }
}

EntityStorage::ComponentGroup::~ComponentGroup()
{
}

EntityStorage::ComponentGroup::BucketPair EntityStorage::ComponentGroup::findBuckets(u32 mask1, u32 mask2)
{
    BucketPair result;
    minitl::tuple<u32, u32> best;
    for (Bucket* bucket = buckets.begin(); bucket != buckets.end(); ++bucket)
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

void EntityStorage::ComponentGroup::mergeEntityOperation(u8* __restrict source, const u8* __restrict merge)
{
    EntityOperation* result = reinterpret_cast<EntityOperation*>(source);
    const EntityOperation* with = reinterpret_cast<const EntityOperation*>(merge);

    u8* buffer = result->componentBuffer;
    u32 bufferSize = 4 + result->size - (u32)sizeof(EntityOperation);
    const u8* mergedBuffer = with->componentBuffer;
    for (u32 i = 0; i < lastComponent-firstComponent; ++i)
    {
        u32 m = 1 << i;
        if (result->componentMaskAdd & m)
        {
            be_assert((with->componentMaskAdd & m) == 0,
                      "Entity %d: adding component %s twice in a frame"
                        | result->entityId | components[i].componentType->fullname());
            u32 size = components[i].size;
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
            u32 sizeToMove = components[i].size;
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
                        | result->entityId | components[i].componentType->fullname());
            result->componentMaskRemove |= m;
        }
    }
}

struct EntityStorage::ComponentGroup::Delta
{
    i32 absoluteOffset;
    u16 added;
    u16 removed;
};

struct EntityStorage::ComponentGroup::Offset
{
    u32 offsetRemoved;
    u32 offsetAdded;
};

struct EntitySort
{
    bool operator()(const EntityOperation& first, const EntityOperation& second)
    {
        return first.componentIndex < second.componentIndex;
    }
};

void EntityStorage::ComponentGroup::moveBucketComponents(u32 componentIndex,
                                                         Bucket* __restrict bucket,
                                                         u8* __restrict operationsRemove,
                                                         u8* __restrict operationsAdd,
                                                         u8* __restrict operationsEnd,
                                                         const u8* __restrict componentBuffer)
{
    be_forceuse(componentIndex);
    be_forceuse(bucket);
    be_forceuse(operationsRemove);
    be_forceuse(operationsAdd);
    be_forceuse(operationsEnd);
    be_forceuse(componentBuffer);

    minitl::sort((EntityOperation*)operationsRemove, (EntityOperation*)operationsAdd, EntitySort());

    // move added components in place of removed ones

    // remove remaining components

    // add remaining components
}

void EntityStorage::ComponentGroup::moveComponents(u32 componentIndex, Bucket* begin, Bucket* end,
                                                   u8* __restrict operations,
                                                   Offset* __restrict operationOffsetPerBucket,
                                                   Delta* __restrict deltas,
                                                   const u8* __restrict componentBuffer)
{
    be_info("moving buckets %d - %d" | begin - buckets.begin() | end - buckets.begin());
    u32 componentCount = components.size();
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
                             operationAddBuffer, operationEndBuffer, componentBuffer);
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
                             operationAddBuffer, operationEndBuffer, componentBuffer);
    }
    if (lowerBound == upperBound)
    {
        minitl::size_type bucketIndex = minitl::distance(begin, upperBound);
        Offset operationOffset = operationOffsetPerBucket[bucketIndex];
        Offset operationOffsetEnd = operationOffsetPerBucket[bucketIndex+1];
        u8* operationRemoveBuffer = operations + operationOffset.offsetRemoved;
        u8* operationAddBuffer = operations + operationOffset.offsetAdded;
        u8* operationEndBuffer = operations + operationOffsetEnd.offsetRemoved;
        moveBucketComponents(componentIndex, lowerBound, operationRemoveBuffer,
                             operationAddBuffer, operationEndBuffer, componentBuffer);
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

void EntityStorage::ComponentGroup::runEntityOperations(weak<EntityStorage> storage,
                                                        u8* __restrict buffer,
                                                        u8* __restrict componentBuffer)
{
    const u32 componentCount = components.size();
    Delta* deltas = (Delta*)malloca(sizeof(Delta) * buckets.size() * componentCount);
    memset(deltas, 0, sizeof(Delta) * buckets.size() * componentCount);
    u32 scratchBufferSize = sizeof(Offset) * (buckets.size() + 1) + sizeof(u32);
    u8* scratchBuffer = (u8*)malloca(scratchBufferSize);
    memset(scratchBuffer, 0, scratchBufferSize);
    Offset* operationOffsetsPerBucket = (Offset*)scratchBuffer;
    Offset* operationSizePerBucket = (Offset*)(scratchBuffer + sizeof(u32));
    u8* destination = buffer;
    u8* destinationEnd = destination;

    u8* source = componentOperations;
    u8* end = source + operationOffset;
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
            result->bucketOrigin = be_checked_numcast<u16>(insertion.first - buckets.begin());
            result->bucketDestination = be_checked_numcast<u16>(insertion.second - buckets.begin());
            result->componentStorage = be_checked_numcast<u32>(componentBackup - componentBuffer);
            be_assert(componentBackup + componentsTotalSize < componentBuffer + storage->m_operationAllocator->blockSize(),
                      "ran out of component backup space");
            Entity e = {result->entityId};
            const EntityInfo& info = storage->getEntityInfo(e);
            componentBackup += storage->store(info, componentBackup, firstComponent,
                                              insertion.first->acceptMask & maskAfter);
            u32 indexSourceBucket = be_checked_numcast<u32>(insertion.first - buckets.begin());
            u32 indexDestinationBucket = be_checked_numcast<u32>(insertion.second-buckets.begin());
            if (insertion.first != insertion.second)
            {
                for (u32 i = 0; i < componentCount; ++i)
                {
                    if ((insertion.first->acceptMask >> i) & 0x1)
                    {
                        deltas[componentCount*(insertion.first - buckets.begin()) + i].removed ++;
                    }
                    if ((insertion.second->acceptMask >> i) & 0x1)
                    {
                        deltas[componentCount*(insertion.second - buckets.begin()) + i].added ++;
                    }
                }

                if (insertion.first->acceptMask != 0)
                {
                    u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperation) - 4);
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
                for (Bucket* bucket = buckets.end() - componentCount - 1;
                     bucket != buckets.end() - 1;
                     ++bucket, ++componentIndex)
                {
                    be_assert(bucket->maskSize == 1, "invalid bucket");
                    u32 bucketIndex = be_checked_numcast<u32>(bucket - buckets.begin());
                    if (bucket->acceptMask & remainderAdd)
                    {
                        deltas[bucketIndex*componentCount + componentIndex].added ++;
                        u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperation) - 4);
                        operationSize += components[componentIndex].size;
                        operationSizePerBucket[bucketIndex].offsetAdded += operationSize;
                    }
                }
            }
            for (u32 componentIndex = 0, bucketIndex = buckets.size() - 1 - componentCount;
                 remainderRemoved != 0;
                 remainderRemoved >>=1, componentIndex++, bucketIndex++)
            {
                if (remainderRemoved & 0x1)
                {
                    deltas[bucketIndex*componentCount + componentIndex].added ++;
                    u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperation) - 4);
                    operationSizePerBucket[bucketIndex].offsetRemoved += operationSize;
                }
            }

            destinationEnd += result->size;
            result = reinterpret_cast<EntityOperation*>(destinationEnd);
        }
    }

    /* sort operations per bucket */
    destination = componentOperations;

    {
        u32 offset = operationSizePerBucket[0].offsetRemoved;
        u32 size = operationSizePerBucket[0].offsetAdded;
        operationSizePerBucket[0].offsetRemoved = 0;
        operationSizePerBucket[0].offsetAdded = offset;
        offset += size;
        for (u32 i = 1; i < buckets.size(); ++i)
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
            Bucket& bucketOrigin = buckets[operation->bucketOrigin];
            Bucket& bucketDestination = buckets[operation->bucketDestination];

            if (bucketOrigin.acceptMask != 0)
            {
                u32 sizeRemove = be_checked_numcast<u32>(sizeof(EntityOperation) - 4);
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
        u32 remainderAdd = maskAfter & ~buckets[operation->bucketDestination].acceptMask;
        if (remainderAdd)
        {
            u32 component = 0;
            const u8* componentBuffer = operation->componentBuffer;
            for (Bucket* bucket = buckets.end() - componentCount - 1;
                 bucket != buckets.end() - 1;
                 ++bucket, ++component)
            {
                be_assert(bucket->maskSize == 1, "invalid bucket");
                u32 index = be_checked_numcast<u32>(bucket - buckets.begin());
                if (bucket->acceptMask & remainderAdd)
                {
                    u32& offset = operationSizePerBucket[index].offsetAdded;
                    memcpy(destination + offset, current, sizeof(EntityOperation) - 4);
                    EntityOperation* result = reinterpret_cast<EntityOperation*>(destination + offset);
                    result->componentMaskAdd = bucket->acceptMask;
                    result->componentMaskRemove = 0;
                    u32 operationSize = be_checked_numcast<u32>(sizeof(EntityOperation) - 4)
                                      + components[component].size;
                    result->size = operationSize;
                    offset += sizeof(EntityOperation) - 4;
                    memcpy(destination + offset, componentBuffer, components[component].size);
                    offset += components[component].size;
                    componentBuffer += components[component].size;
                }
                else if (bucket->acceptMask & operation->componentMaskAdd)
                {
                    componentBuffer += components[component].size;
                }
            }
        }
        u32 remainderRemoved = operation->componentMaskRemove & ~buckets[operation->bucketOrigin].acceptMask;
        if (remainderRemoved)
        {
            u32 offset = 0;
            for (Bucket* bucket = buckets.end() - componentCount - 1; bucket != buckets.end() - 1; ++bucket, ++offset)
            {
                be_assert(bucket->maskSize == 1, "invalid bucket");
                u32 index = be_checked_numcast<u32>(bucket - buckets.begin());
                if (bucket->acceptMask & remainderRemoved)
                {
                    u32& offset = operationSizePerBucket[index].offsetRemoved;
                    memcpy(destination + offset, current, sizeof(EntityOperation) - 4);
                    EntityOperation* operationRemove = reinterpret_cast<EntityOperation*>(destination + offset);
                    operationRemove->componentMaskAdd = 0;
                    operationRemove->componentMaskRemove = bucket->acceptMask;
                    operationRemove->size = be_checked_numcast<u32>(sizeof(EntityOperation) - 4);
                    offset += sizeof(EntityOperation) - 4;
                }
            }
        }
        current = current + operation->size;
    }

    for (u32 componentIndex = 0; componentIndex < components.size(); ++componentIndex)
    {
        moveComponents(componentIndex, buckets.begin(), buckets.end(), destination,
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
         current < destination + operationSizePerBucket[buckets.size()-1].offsetRemoved;
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
                        | components[c].componentType->fullname() | *reinterpret_cast<const u32*>(buffer));
                buffer += components[c].componentType->size;
            }
            if (operation->componentMaskRemove & m)
            {
                be_info("  removed %s" | components[c].componentType->fullname());
            }
        }
    }
#endif

    freea(scratchBuffer);
    freea(deltas);

    operationOffset = 0;
}



EntityStorage::ComponentIndex::ComponentIndex()
    :   group((u16)~0)
    ,   relativeIndex((u8)~0)
    ,   absoluteIndex((u8)~0)
{
}

EntityStorage::ComponentIndex::ComponentIndex(u32 group, u32 relativeIndex, u32 absoluteIndex)
    :   group(be_checked_numcast<u16>(group))
    ,   relativeIndex(be_checked_numcast<u8>(relativeIndex))
    ,   absoluteIndex(be_checked_numcast<u8>(absoluteIndex))
{
}

EntityStorage::ComponentIndex::operator void*() const
{
    return (void*)(group != (u16)~0 || relativeIndex != (u8)~0);
}

bool EntityStorage::ComponentIndex::operator!() const
{
    return group == (u16)~0 && relativeIndex == (u8)~0;
}

struct EntityStorage::EntityInfo
{
    union
    {
        u32 next;
        u32 index;
    };
    minitl::bitset<96>  mask;
    u32 componentIndex[1];
};

static const u32 s_usedBit = 0x80000000;
static const u32 s_indexMask = 0x7FFFFFFF;

EntityStorage::EntityStorage(const WorldComposition& composition)
    :   m_task(scoped< Task::Task< Task::MethodCaller<EntityStorage, &EntityStorage::start> > >::create(
                    Arena::task(),
                    "start",
                    Colors::Green::Green,
                    Task::MethodCaller<EntityStorage, &EntityStorage::start>(this)))
    ,   m_freeEntityId(0)
    ,   m_entityAllocator(SystemAllocator::BlockSize_16k, 32)
    ,   m_operationAllocator(0)
    ,   m_entityInfoBuffer((u8**)m_entityAllocator.allocate())
    ,   m_entityCount(0)
    ,   m_entityBufferCount(0)
    ,   m_maxEntityBufferCount(m_entityAllocator.blockSize() / sizeof(EntityInfo*))
    ,   m_bufferCapacity(m_entityAllocator.blockSize() / sizeof(EntityStorage) - 4 * (composition.components.size() - 1))
    ,   m_componentTypes(Arena::game(), composition.components.size())
    ,   m_componentGroups(Arena::game())
    ,   m_components(Arena::game(), composition.components.size())
{
    m_entityInfoBuffer[0] = 0;
    buildGroups(composition);
    for (WorldComposition::ComponentArray::const_iterator it = composition.components.begin();
         it != composition.components.end();
         ++it)
    {
        ComponentIndex index = getComponentIndex(it->first);
        be_assert(index, "component %s not registered" | it->first->fullname());
        m_components[index.absoluteIndex].memory = (u8*)Arena::game().alloc(it->first->size * it->second);
        m_components[index.absoluteIndex].backLink = (u32*)Arena::game().alloc(sizeof(u32) * it->second);
        m_components[index.absoluteIndex].current = 0;
        m_components[index.absoluteIndex].maximum = it->second;
        m_components[index.absoluteIndex].elementSize = it->first->size;
    }
}

EntityStorage::~EntityStorage()
{
    //be_assert(m_entityCount == 0,
    //          "%d entities still spawned when deleting world" | m_entityCount);
    for (u8** buffer = m_entityInfoBuffer; *buffer != 0; ++buffer)
    {
        m_entityAllocator.free(*buffer);
    }
    for (minitl::array<ComponentStorage>::const_iterator it = m_components.begin();
         it != m_components.end();
         ++it)
    {
        Arena::game().free(it->memory);
        Arena::game().free(it->backLink);
    }
    for (minitl::vector<ComponentGroup>::const_iterator it = m_componentGroups.begin();
         it != m_componentGroups.end();
         ++it)
    {
        m_operationAllocator->free(it->componentOperations);
        Arena::game().free(it->componentCounts);
        Arena::game().free(it->componentOffsets);
    }
    delete m_operationAllocator;
    m_entityAllocator.free(m_entityInfoBuffer);
}

weak<Task::ITask> EntityStorage::initialTask() const
{
    return m_task;
}

void EntityStorage::buildGroups(const WorldComposition& composition)
{
    minitl::vector<GroupInfo> groups(Arena::temporary(), composition.partitions.begin(), composition.partitions.end());

    if (groups.size() > 1)
    {
        for (minitl::vector<GroupInfo>::reverse_iterator it = groups.rbegin()+1; it != groups.rend(); ++it)
        {
            for (minitl::vector<GroupInfo>::iterator it2 = it-1; it2 != groups.end(); /*nothing*/)
            {
                if (it->tryMerge(*it2))
                {
                    it2 = groups.erase(it2);
                }
                else
                {
                    it2 ++;
                }
            }
        }
    }

    u32 groupIndex = 0;
    u32 totalComponents = 0;
    m_operationAllocator = new SystemAllocator(SystemAllocator::BlockSize_64k, 2*be_checked_numcast<u32>(groups.size()));
    m_componentGroups.reserve(groups.size());
    for (minitl::vector<GroupInfo>::const_iterator group = groups.begin(); group != groups.end(); ++group, ++groupIndex)
    {
        minitl::vector< raw<const RTTI::Class> > registeredComponents(Arena::temporary());
        u32 componentIndex = 0;
        u32 totalSize = 0;
        for (minitl::vector< raw<const RTTI::Class> >::const_iterator component = group->components.begin();
             component != group->components.end();
              ++component, ++componentIndex)
        {
            for (WorldComposition::ComponentArray::const_iterator ci = composition.components.begin();
                 ci != composition.components.end();
                 ++ci)
            {
                if (ci->first == *component)
                {
                    registeredComponents.push_back(*component);
                    registerType(*component, groupIndex, componentIndex, totalComponents + componentIndex, ci->second);
                    totalSize += (*component)->size;
                }
            }
        }
        totalComponents +=componentIndex;

        minitl::vector<u32> masks(Arena::temporary());
        for (WorldComposition::PartitionVector::const_iterator partition = group->partitions.begin();
             partition != group->partitions.end();
             ++partition)
        {
            u32 mask = buildMask(*partition);
            minitl::vector<u32> previousMasks(Arena::temporary(), masks.rbegin(), masks.rend());
            for (minitl::vector<u32>::const_iterator m = previousMasks.begin(); m != previousMasks.end(); ++m)
            {
                if (find(mask | *m, masks.begin(), masks.end()) == masks.end())
                {
                    masks.push_back(mask | *m);
                }
            }
            masks.push_back(mask);
        }

        for (u32 i = 0; i < componentIndex; ++i)
        {
            masks.push_back(1 << i);
        }
        masks.push_back(0);
        
        u32* componentCounts = (u32*)Arena::game().alloc(sizeof(u32) * componentIndex * masks.size());
        for (u32 i = 0; i < componentIndex * masks.size(); ++i)
        {
            componentCounts[i] = 0;
        }
        u32* componentOffsets = (u32*)Arena::game().alloc(sizeof(u32) * componentIndex * masks.size());
        for (u32 i = 0; i < masks.size(); ++i)
        {
            u32 m = masks[i];
            for (u32 j = 0; j < componentIndex; ++j)
            {
                componentOffsets[componentIndex*i + j] = m & 0x1;
                m >>= 1;
            }
        }
        u32 componentSize = be_checked_numcast<u32>(group->components.size());
        u32 firstComponent = totalComponents - componentSize;
        m_componentGroups.push_back(ComponentGroup(firstComponent, componentSize, totalSize, componentCounts,
                                                   componentOffsets, masks, (u8*)m_operationAllocator->allocate()));
    }

    for (minitl::array<ComponentInfo>::const_iterator it = m_componentTypes.begin(); it != m_componentTypes.end(); ++it)
    {
        ComponentGroup::ComponentInfo& info = m_componentGroups[it->second.group].components[it->second.relativeIndex];
        info.componentType = it->first;
        info.size = it->first->size;
        info.created = getMethodFromClass(it->first, "created");
        if (!info.created)
        {
            be_error("component type %s: invalid signature for method \"created\"" | it->first->fullname());
        }
        info.destroyed = getMethodFromClass(it->first, "destroyed");
        if (!info.destroyed)
        {
            be_error("component type %s: invalid signature for method \"destroyed\"" | it->first->fullname());
        }
    }
}

u32 EntityStorage::buildMask(const minitl::array< raw<const RTTI::Class> >& componentList)
{
    u32 result = 0;
    for (minitl::array< raw<const RTTI::Class> >::const_iterator it = componentList.begin();
         it != componentList.end();
         ++it)
    {
        ComponentIndex index = getComponentIndex(*it);
        result |= (u32)1 << index.relativeIndex;
    }
    return result;
}

void EntityStorage::registerType(raw<const RTTI::Class> componentType, u32 group, u32 relativeIndex,
                                 u32 absoluteIndex, u32 maximumCount)
{
    be_assert(!getComponentIndex(componentType),
              "component type %s is registered twice" | componentType->fullname());
    be_info("component %s: reserving space for %d" | componentType->name | maximumCount);
    ComponentInfo& info = m_componentTypes[absoluteIndex];
    info.first = componentType;
    info.second = ComponentIndex(group, relativeIndex, absoluteIndex);
    info.third = componentType->size;
}

EntityStorage::ComponentIndex EntityStorage::getComponentIndex(raw<const RTTI::Class> componentType) const
{
    for (u32 i = 0; i < m_componentTypes.size(); ++i)
    {
        if (m_componentTypes[i].first == componentType)
        {
            return m_componentTypes[i].second;
        }
    }
    return ComponentIndex();
}

const EntityStorage::ComponentInfo& EntityStorage::getComponentInfo(raw<const RTTI::Class> componentType) const
{
    for (u32 i = 0; i < m_componentTypes.size(); ++i)
    {
        if (m_componentTypes[i].first == componentType)
        {
            return m_componentTypes[i];
        }
    }
    be_notreached();
    return m_componentTypes[0];
}

void EntityStorage::start()
{
    u8* buffer = (u8*)m_operationAllocator->allocate();
    u8* componentBuffer = (u8*)m_operationAllocator->allocate();
    for (minitl::vector<ComponentGroup>::iterator it = m_componentGroups.begin(); it != m_componentGroups.end(); ++it)
    {
        it->runEntityOperations(this, buffer, componentBuffer);
    }
    m_operationAllocator->free(componentBuffer);
    m_operationAllocator->free(buffer);
}

u32 EntityStorage::getEntityInfoSize() const
{
    return be_checked_numcast<u32>(sizeof(EntityInfo) + sizeof(u32) * (m_componentTypes.size()-1));
}

EntityStorage::EntityInfo& EntityStorage::getEntityInfo(Entity e)
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    return *(EntityInfo*)(m_entityInfoBuffer[bufferIndex] + bufferOffset*getEntityInfoSize());
}

const EntityStorage::EntityInfo& EntityStorage::getEntityInfo(Entity e) const
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    return *(const EntityInfo*)(m_entityInfoBuffer[bufferIndex] + bufferOffset*getEntityInfoSize());
}

Entity EntityStorage::spawn()
{
    Entity e = { m_freeEntityId };

    u32 bufferIndex = e.id >> 16;
    be_assert(bufferIndex <= m_entityBufferCount,
              "invalid buffer index: %d (range: %d, %d)" | bufferIndex | 0 | m_entityBufferCount);
    if (bufferIndex == m_entityBufferCount)
    {
        be_assert(m_entityInfoBuffer[bufferIndex] == 0,
                  "buffer index %d inconsistent" | bufferIndex);
        u8* buffer = (u8*)m_entityAllocator.allocate();
        u32 index = (bufferIndex << 16);
        const u32 infoSize = getEntityInfoSize();
        for (u32 i = 0; i < m_bufferCapacity; ++i)
        {
            EntityInfo& info = *(EntityInfo*)(buffer + i * infoSize);
            info.next = index + i + 1;
            info.mask = minitl::bitset<96>();
            for (u32 j = 0; j < m_componentTypes.size(); ++j)
            {
                info.componentIndex[j] = 0;
            }
        }
        EntityInfo& info = *(EntityInfo*)(buffer + (m_bufferCapacity-1) * infoSize);
        info.next = (1 + bufferIndex) << 16;

        m_entityInfoBuffer[bufferIndex] = buffer;
        m_entityInfoBuffer[bufferIndex+1] = 0;
        m_entityBufferCount++;
    }

    EntityInfo& entityInfo = getEntityInfo(e);
    be_assert((entityInfo.index & s_usedBit) == 0,
              "Entity %s is already in use; entity buffer inconsistent" | e.id);
    m_freeEntityId = entityInfo.next;
    entityInfo.index = s_usedBit;
    entityInfo.mask = minitl::bitset<96>();
    ++ m_entityCount;
    return e;
}

void EntityStorage::unspawn(Entity e)
{
    EntityInfo& info = getEntityInfo(e);
    // unspawn components
    info.next = m_freeEntityId;
    info.mask = minitl::bitset<96>();
    m_freeEntityId = e.id;
    -- m_entityCount;
}

void EntityStorage::addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType)
{
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id,
                      return);
    ComponentIndex componentIndex = getComponentIndex(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(),
                      return);

    ComponentGroup& group = m_componentGroups[componentIndex.group];
    u32 componentSize = group.components[componentIndex.relativeIndex].size;
    u32 size = be_checked_numcast<u32>(sizeof(EntityOperation)) + componentSize - 4;
    u32 offset = (group.operationOffset += size) - size;
    be_assert_recover(offset < m_operationAllocator->blockSize(),
                      "Entity %d: could not add component of type %s: ran out of buffer"
                        | e.id | componentType->fullname(),
                      return);
    EntityOperation* buffer = reinterpret_cast<EntityOperation*>(group.componentOperations + offset);
    buffer->entityId = e.id;
    buffer->size = size;
    buffer->originalMask = info.mask(group.firstComponent, group.lastComponent);
    buffer->componentMaskAdd = 1 << componentIndex.relativeIndex;
    buffer->componentMaskRemove = 0;
    buffer->componentStorage = 0;
    buffer->bucketOrigin = 0;
    buffer->bucketDestination = 0;
    memcpy(buffer->componentBuffer, &c, componentSize);

    info.mask[componentIndex.absoluteIndex] = true;
}

void EntityStorage::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id,
                      return);
    ComponentIndex componentIndex = getComponentIndex(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(),
                      return);
    be_assert_recover(info.mask[componentIndex.absoluteIndex],
                      "entity %d does not has a component %s" | e.id | componentType->fullname(),
                      return);

    ComponentGroup& group = m_componentGroups[componentIndex.group];
    u32 size = be_checked_numcast<u32>(sizeof(EntityOperation)) - 4;
    u32 offset = (group.operationOffset += size) - size;
    be_assert_recover(offset < m_operationAllocator->blockSize(),
                      "Entity %d: could not add component of type %s: ran out of buffer"
                        | e.id | componentType->fullname(),
                      return);
    EntityOperation* buffer = reinterpret_cast<EntityOperation*>(group.componentOperations + offset);
    buffer->entityId = e.id;
    buffer->size = size;
    buffer->originalMask = info.mask(group.firstComponent, group.lastComponent);
    buffer->componentMaskAdd = 0;
    buffer->componentMaskRemove = 1 << componentIndex.relativeIndex;
    buffer->componentStorage = 0;
    buffer->bucketOrigin = 0;
    buffer->bucketDestination = 0;

    info.mask[componentIndex.absoluteIndex] = false;
}

bool EntityStorage::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    const EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id,
                      return false);
    ComponentIndex componentIndex = getComponentIndex(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(),
                      return false);
    return info.mask[componentIndex.absoluteIndex];
}

RTTI::Value EntityStorage::getComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    be_assert(hasComponent(e, componentType),
              "Entity %s does not have a component of type %s" | e.id | componentType->fullname());
    const EntityInfo& info = getEntityInfo(e);
    const ComponentIndex componentTypeIndex = getComponentIndex(componentType);
    const u32 componentIndex = info.componentIndex[componentTypeIndex.absoluteIndex];
    const ComponentStorage& storage = m_components[componentTypeIndex.absoluteIndex];
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(),
                      return RTTI::Value());
    be_assert(componentIndex < storage.current, "component index %d out of range" | componentIndex);
    return RTTI::Value(RTTI::Type::makeType(componentType, RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Const),
                       storage.memory + componentIndex*componentType->size);
}

EntityStorage::ComponentGroup& EntityStorage::getComponentGroup(ComponentIndex componentIndex)
{
    be_assert(componentIndex,
              "invalid component index given to getComponentGroup");
    be_assert(componentIndex.group < m_componentGroups.size(),
              "invalid component index given to getComponentGroup");
    return m_componentGroups[componentIndex.group];
}

u32 EntityStorage::store(const EntityInfo& info, u8* buffer, u32 firstComponent, u32 mask)
{
    u32 result = 0;
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | (info.index & s_indexMask),
                      return result);
    for (u32 c = firstComponent; mask; c++, mask >>= 1)
    {
        if (mask & 1)
        {
            u32 size = m_componentTypes[c].third;
            u32 offset = info.componentIndex[c];
            be_assert_recover(offset < m_components[c].current,
                              "entity id %d: component %s index %d out of range (%d-%d)"
                                | (info.index & s_indexMask) | m_componentTypes[c].first->fullname()
                                | offset | 0 | m_components[c].current,
                              continue);
            memcpy(buffer, m_components[c].memory + offset, size);
            buffer += size;
            result += size;
        }
    }
    return result;
}

void EntityStorage::restore(const EntityInfo& info, u8* buffer, u32 firstComponent, u32 mask)
{
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | (info.index & s_indexMask),
                      return);
    for (u32 c = firstComponent; mask; c++, mask >>= 1)
    {
        if (mask & 1)
        {
            u32 size = m_componentTypes[c].third;
            u32 offset = info.componentIndex[c];
            be_assert_recover(offset < m_components[c].current,
                              "entity id %d: component %s index %d out of range (%d-%d)"
                                | (info.index & s_indexMask) | m_componentTypes[c].first->fullname()
                                | offset | 0 | m_components[c].current,
                              continue);
            memcpy(m_components[c].memory + offset, buffer, size);
            buffer += size;
        }
    }
}

u32 EntityStorage::getComponentIndex(Entity e, const ComponentGroup& group,
                                     const ComponentGroup::Bucket& bucket) const
{
    const EntityInfo& info = getEntityInfo(e);
    u32 index = bucket.firstComponent;
    u32 result = info.componentIndex[group.firstComponent + index];
    be_assert(result < bucket.componentCounts[index],
              "component index %d is not in the bucket"
                | group.firstComponent+index);
#if BE_ENABLE_ASSERT
    u32 indexInBucket = result - bucket.componentCounts[index];
    const u32 mask = bucket.acceptMask;
    for (u32 i = group.firstComponent; i < group.lastComponent; ++i)
    {
        if (mask & (1 << (i-group.firstComponent)))
        {
            u32 r = info.componentIndex[i];
            u32 cInBucket = r - bucket.componentCounts[i-group.firstComponent];
            be_assert(cInBucket == indexInBucket, "inconsistent offset in buffer");
        }
    }
#endif
    return result;
}

}}
