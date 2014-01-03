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
    minitl::bitset<96> componentMaskAdd;
    minitl::bitset<96> componentMaskRemove;
    u8 componentBuffer[1];
};

static raw<const RTTI::Method::Overload> getMethodFromClass(raw<const RTTI::Class> type, istring name)
{
    raw<const RTTI::Method> method = type->getMethod(name);
    be_assert_recover(method, "could not locate method \"%s\" for component %s" | name | type->fullname(), return raw<const RTTI::Method::Overload>());
    for (raw<const RTTI::Method::Overload> overload = method->overloads; overload; overload = overload->next)
    {
        if (overload->parameterCount == 1
         && RTTI::Type::makeType(type, RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable).isA(overload->params->type))
        {
            return overload;
        }
    }
    return raw<const RTTI::Method::Overload>();
}

struct GroupInfo
{
    minitl::vector< raw<const RTTI::Class> >                    components;
    minitl::vector< minitl::array< raw<const RTTI::Class> > >   partitions;

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
            for (minitl::vector< raw<const RTTI::Class> >::const_iterator it2 = components.begin(); it2 != components.end(); ++it2)
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
            for (minitl::vector< raw<const RTTI::Class> >::const_iterator component = otherGroup.components.begin();
                 component != otherGroup.components.end();
                 ++component)
            {
                if (find(*component, components.begin(), components.end()) == components.end())
                {
                    components.push_back(*component);
                }
            }
            for (minitl::vector< minitl::array< raw<const RTTI::Class> > >::const_iterator partition = otherGroup.partitions.begin();
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

}

EntityStorage::ComponentGroup::Bucket::Bucket()
    :   componentCounts(0)
    ,   acceptMask(0)
    ,   maskSize(0)
{
}

EntityStorage::ComponentGroup::Bucket::Bucket(u32* componentCounts, u32 acceptMask)
    :   componentCounts(componentCounts)
    ,   acceptMask(acceptMask)
    ,   maskSize(bitCount(acceptMask))
{
}


EntityStorage::ComponentGroup::ComponentGroup(u32 firstComponent, u32 componentCount, u32 componentsTotalSize, u32* componentOffsets, const minitl::vector<u32>& bucketMasks)
    :   buckets(Arena::game(), (u32)bucketMasks.size())
    ,   firstComponent(firstComponent)
    ,   lastComponent(firstComponent + componentCount)
    ,   componentsTotalSize(componentsTotalSize)
    ,   componentCounts(componentOffsets)
{
    u32 index = 0;
    for (minitl::vector<u32>::const_iterator it = bucketMasks.begin(); it != bucketMasks.end(); ++it, ++index)
    {
        buckets[index] = Bucket(componentCounts + componentCount * index, *it);
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
    ,   m_entityInfoBuffer((u8**)m_entityAllocator.allocate())
    ,   m_entityCount(0)
    ,   m_entityBufferCount(0)
    ,   m_maxEntityBufferCount(m_entityAllocator.blockSize() / sizeof(EntityInfo*))
    ,   m_bufferCapacity(m_entityAllocator.blockSize() / sizeof(EntityStorage) - 4 * (composition.components.size() - 1))
    ,   m_operationBuffer((u8*)m_entityAllocator.allocate())
    ,   m_endOperation(i_u32::Zero)
    ,   m_componentTypes(Arena::game(), composition.components.size())
    ,   m_componentGroups(Arena::game())
    ,   m_componentCountsList(Arena::game())
    ,   m_components(Arena::game(), composition.components.size())
{
    m_entityInfoBuffer[0] = 0;
    buildGroups(composition);
    for (minitl::array< minitl::tuple< raw<const RTTI::Class>, u32 > >::const_iterator it = composition.components.begin();
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
    be_assert(m_entityCount == 0,
              "%d entities still spawned when deleting world" | m_entityCount);
    for (u8** buffer = m_entityInfoBuffer; *buffer != 0; ++buffer)
    {
        m_entityAllocator.free(*buffer);
    }
    for (minitl::vector<u32*>::const_iterator it = m_componentCountsList.begin(); it != m_componentCountsList.end(); ++it)
    {
        Arena::game().free(*it);
    }
    for (minitl::array<ComponentStorage>::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
    {
        Arena::game().free(it->memory);
        Arena::game().free(it->backLink);
    }
    m_entityAllocator.free(m_operationBuffer);
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
    for (minitl::vector<GroupInfo>::const_iterator group = groups.begin(); group != groups.end(); ++group, ++groupIndex)
    {
        minitl::vector< raw<const RTTI::Class> > registeredComponents(Arena::temporary());
        u32 componentIndex = 0;
        u32 totalSize = 0;
        for (minitl::vector< raw<const RTTI::Class> >::const_iterator component = group->components.begin();
             component != group->components.end();
              ++component, ++componentIndex)
        {
            for (minitl::array< minitl::tuple< raw<const RTTI::Class>, u32 > >::const_iterator ci = composition.components.begin();
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
        for (minitl::vector< minitl::array< raw<const RTTI::Class> > >::const_iterator partition = group->partitions.begin();
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
        m_componentCountsList.push_back(componentCounts);
        u32 componentSize = be_checked_numcast<u32>(group->components.size());
        u32 firstComponent = totalComponents - componentSize;
        m_componentGroups.push_back(ComponentGroup(firstComponent, componentSize, totalSize, componentCounts, masks));
    }
}

u32 EntityStorage::buildMask(const minitl::array< raw<const RTTI::Class> >& componentList)
{
    u32 result = 0;
    for (minitl::array< raw<const RTTI::Class> >::const_iterator it = componentList.begin(); it != componentList.end(); ++it)
    {
        ComponentIndex index = getComponentIndex(*it);
        result |= (u32)1 << index.relativeIndex;
    }
    return result;
}

void EntityStorage::registerType(raw<const RTTI::Class> componentType, u32 group, u32 relativeIndex, u32 absoluteIndex, u32 maximumCount)
{
    be_assert(!getComponentIndex(componentType),
              "component type %s is registered twice" | componentType->fullname());
    be_info("component %s: reserving space for %d" | componentType->name | maximumCount);
    ComponentInfo& info = m_componentTypes[absoluteIndex];
    info.first = componentType;
    info.second = ComponentIndex(group, relativeIndex, absoluteIndex);
    info.third = componentType->size;

    info.fourth = getMethodFromClass(componentType, "created");
    if (!info.fourth)
    {
        be_error("component type %s: invalid signature for method \"created\"" | componentType->fullname());
    }
    info.fifth = getMethodFromClass(componentType, "destroyed");
    if (!info.fifth)
    {
        be_error("component type %s: invalid signature for method \"destroyed\"" | componentType->fullname());
    }
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
    u8* endBuffer = mergeEntityOperations(m_operationBuffer, m_operationBuffer + m_endOperation);
    m_endOperation = 0;
}

u32 EntityStorage::getEntityInfoSize() const
{
    return be_checked_numcast<u32>(sizeof(EntityInfo) + sizeof(u32) * (m_componentTypes.size()-1));
}

EntityStorage::EntityInfo& EntityStorage::getEntityInfo(BugEngine::World::Entity e)
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    return *(EntityInfo*)(m_entityInfoBuffer[bufferIndex] + bufferOffset*getEntityInfoSize());
}

const EntityStorage::EntityInfo& EntityStorage::getEntityInfo(BugEngine::World::Entity e) const
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
    be_forceuse(c);
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id,
                      return);
    ComponentIndex componentIndex = getComponentIndex(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(),
                      return);

    u32 componentSize = m_componentTypes[componentIndex.absoluteIndex].third;
    u32 size = be_checked_numcast<u32>(sizeof(EntityOperation)) + componentSize - 1;
    u32 offset = (m_endOperation += size) - size;
    be_assert_recover(offset < m_entityAllocator.blockSize(),
                      "Entity %d: could not add component of type %s: ran out of buffer" | e.id | componentType->fullname(),
                      return);
    EntityOperation* buffer = reinterpret_cast<EntityOperation*>(m_operationBuffer + offset);
    buffer->entityId = e.id;
    buffer->size = size;
    buffer->componentMaskAdd = minitl::bitset<96>();
    buffer->componentMaskRemove = minitl::bitset<96>();
    buffer->componentMaskAdd[componentIndex.absoluteIndex] = true;
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

    u32 size = be_checked_numcast<u32>(sizeof(EntityOperation)) - 1;
    u32 offset = (m_endOperation += size) - size;
    be_assert_recover(offset < m_entityAllocator.blockSize(),
                      "Entity %d: could not add component of type %s: ran out of buffer" | e.id | componentType->fullname(),
                      return);
    EntityOperation* buffer = reinterpret_cast<EntityOperation*>(m_operationBuffer + offset);
    buffer->entityId = e.id;
    buffer->size = size;
    buffer->componentMaskAdd = minitl::bitset<96>();
    buffer->componentMaskRemove = minitl::bitset<96>();
    buffer->componentMaskRemove[componentIndex.absoluteIndex] = true;

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

RTTI::Value EntityStorage::getComponent(BugEngine::World::Entity e, raw<const RTTI::Class> componentType) const
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

void EntityStorage::mergeEntityOperation(u8* source, const u8* merge)
{
    EntityOperation* result = reinterpret_cast<EntityOperation*>(source);
    const EntityOperation* with = reinterpret_cast<const EntityOperation*>(merge);

    u8* buffer = result->componentBuffer;
    u32 bufferSize = 1 + result->size - (u32)sizeof(EntityOperation);
    const u8* mergedBuffer = with->componentBuffer;
    for (u32 i = 0; i < m_componentTypes.size(); ++i)
    {
        if (result->componentMaskAdd[i])
        {
            be_assert(!with->componentMaskAdd[i],
                      "Entity %d: adding component %s twice in a frame" | result->entityId | m_componentTypes[i].first->fullname());
            u32 size = m_componentTypes[i].third;
            if (with->componentMaskRemove[i])
            {
                result->size -= size;
                bufferSize -= size;
                memmove(buffer, buffer + size, bufferSize);
                result->componentMaskAdd[i] = false;
            }
            else
            {
                buffer += size;
                bufferSize -= size;
            }
        }
        else if (with->componentMaskAdd[i])
        {
            result->componentMaskAdd[i] = true;
            u32 sizeToMove = m_componentTypes[i].third;
            if (bufferSize)
            {
                memmove(buffer + sizeToMove, buffer, bufferSize);
            }
            memcpy(buffer, mergedBuffer, sizeToMove);
            mergedBuffer += sizeToMove;
            buffer += sizeToMove;
            result->size += sizeToMove;
        }
        else if (with->componentMaskRemove[i])
        {
            be_assert(!result->componentMaskRemove[i],
                      "Entity %d: adding component %s twice in a frame" | result->entityId | m_componentTypes[i].first->fullname());
            result->componentMaskRemove[i] = true;
        }
    }
}

void EntityStorage::runEntityOperations(u8* source, u8* end)
{
    u32** deltas = (u32**)malloca(sizeof(u32*) * m_componentGroups.size());
    for (u32 i = 0; i < m_componentGroups.size(); ++i)
    {
        deltas[i] = (u32*)malloca(sizeof(u32) * m_componentGroups[i].buckets.size());
    }
    u32** groupOperationSize = (u32**)malloca(sizeof(u32*) * m_componentGroups.size());
    for (u32 i = 0; i < m_componentGroups.size(); ++i)
    {
        groupOperationSize[i] = (u32*)malloca(sizeof(u32) * m_componentGroups[i].buckets.size());
    }

    u8* destination = (u8*)m_entityAllocator.allocate();
    u8* destinationEnd = destination;

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
            destinationEnd += result->size;
            result = reinterpret_cast<EntityOperation*>(destinationEnd);
        }
    }



    m_entityAllocator.free(destination);
    for (u32 i = be_checked_numcast<u32>(m_componentGroups.size()); i > 0; --i)
    {
        freea(groupOperationSize[i-1]);
    }
    freea(groupOperationSize);
    for (u32 i = be_checked_numcast<u32>(m_componentGroups.size()); i > 0; --i)
    {
        freea(deltas[i-1]);
    }
    freea(deltas);
}

}}
