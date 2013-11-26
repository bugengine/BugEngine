/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <scheduler/task/method.hh>
#include    <minitl/bitset.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/typeinfo.hh>

namespace BugEngine { namespace World
{

template< typename ITERATOR, typename T >
ITERATOR find(const T& t, ITERATOR begin, ITERATOR end)
{
    for (ITERATOR it = begin; it != end; ++it)
    {
        if (*it == t)
            return it;
    }
    return end;
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


EntityStorage::Bucket::Bucket()
    :   acceptMask(0)
    ,   componentCounts(0)
{
}

EntityStorage::Bucket::Bucket(u32* componentCounts, u32 acceptMask)
    :   acceptMask(acceptMask)
    ,   componentCounts(componentCounts)
{
}

EntityStorage::Bucket::~Bucket()
{
}

EntityStorage::ComponentGroup::ComponentGroup(u32 componentCount, u32* componentCounts, const minitl::vector<u32>& bucketMasks)
    :   buckets(Arena::game(), (u32)bucketMasks.size())
    ,   componentCounts(componentCounts)
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



struct EntityStorage::EntityInfo
{
    union
    {
        u32 next;
        u32 index;
    };
    minitl::bitset<96>  mask;
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
    ,   m_entityInfoBuffer((EntityInfo**)m_entityAllocator.allocate())
    ,   m_entityCount(0)
    ,   m_entityBufferCount(0)
    ,   m_maxEntityBufferCount(m_entityAllocator.blockSize() / sizeof(EntityInfo*))
    ,   m_bufferCapacity(m_entityAllocator.blockSize() / sizeof(EntityInfo))
    ,   m_componentTypes(Arena::game(), composition.components.size())
    ,   m_componentGroups(Arena::game())
    ,   m_componentCountsList(Arena::game())
{
    m_entityInfoBuffer[0] = 0;
    buildGroups(composition);
}

EntityStorage::~EntityStorage()
{
    be_assert(m_entityCount == 0,
              "%d entities still spawned when deleting world" | m_entityCount);
    for (EntityInfo** buffer = m_entityInfoBuffer; *buffer != 0; ++buffer)
    {
        m_entityAllocator.free(*buffer);
    }
    for (minitl::vector<u32*>::const_iterator it = m_componentCountsList.begin(); it != m_componentCountsList.end(); ++it)
    {
        Arena::game().free(*it);
    }
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
    for (minitl::vector<GroupInfo>::const_iterator group = groups.begin(); group != groups.end(); ++group, ++groupIndex)
    {
        minitl::vector< raw<const RTTI::Class> > registeredComponents(Arena::temporary());
        char message[1024] = {0};
        u32 componentIndex = 0;
        for (minitl::vector< raw<const RTTI::Class> >::const_iterator component = group->components.begin();
             component != group->components.end();
              ++component, ++componentIndex)
        {
            strcat(message, (*component)->name.c_str());
            if (component != group->components.end())
            {
                strcat(message, "+");
            }
            for (minitl::array< minitl::pair< raw<const RTTI::Class>, u32 > >::const_iterator ci = composition.components.begin();
                 ci != composition.components.end();
                 ++ci)
            {
                if (ci->first == *component)
                {
                    registeredComponents.push_back(*component);
                    registerType(*component, groupIndex, componentIndex, ci->second);
                }
            }
        }

        be_info("Group: %s" | message);
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

        for (minitl::vector<u32>::const_iterator mask = masks.begin(); mask != masks.end(); ++mask)
        {
            message[0] = 0;
            for (u32 i = 0; i < componentIndex; ++i)
            {
                if (*mask & (1 << i))
                {
                    strcat(message, registeredComponents[i]->name.c_str());
                    strcat(message, "+");
                }
            }
            be_info(" Bucket: %s" | message);
        }

        u32* componentCounts = (u32*)Arena::game().alloc(sizeof(u32) * componentIndex * masks.size());
        m_componentCountsList.push_back(componentCounts);
        m_componentGroups.push_back(ComponentGroup(be_checked_numcast<u32>(group->components.size()), componentCounts, masks));
    }
}

u32 EntityStorage::buildMask(const minitl::array< raw<const RTTI::Class> >& componentList)
{
    u32 result = 0;
    for (minitl::array< raw<const RTTI::Class> >::const_iterator it = componentList.begin(); it != componentList.end(); ++it)
    {
        ComponentIndex index = indexOf(*it);
        result |= (u32)1 << index.index;
    }
    return result;
}

void EntityStorage::registerType(raw<const RTTI::Class> componentType, u32 group, u32 index, u32 maximumCount)
{
    be_assert(!indexOf(componentType),
              "component type %s is registered twice" | componentType->fullname());
    be_info("component %s: reserving space for %d" | componentType->name | maximumCount);
    m_componentTypes[index].first = componentType;
    m_componentTypes[index].second = ComponentIndex(group, index);
}

EntityStorage::ComponentIndex EntityStorage::indexOf(raw<const RTTI::Class> componentType) const
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

void EntityStorage::start()
{
}

EntityStorage::EntityInfo& EntityStorage::getEntityInfo(BugEngine::World::Entity e)
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    return m_entityInfoBuffer[bufferIndex][bufferOffset];
}

const EntityStorage::EntityInfo& EntityStorage::getEntityInfo(BugEngine::World::Entity e) const
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    return m_entityInfoBuffer[bufferIndex][bufferOffset];
}

Entity EntityStorage::spawn()
{
    Entity e = { m_freeEntityId };

    u32 bufferIndex = e.id >> 16;
    be_assert(bufferIndex <= m_entityBufferCount,
              "invalid buffer index: %d (range: %d, %d)" | bufferIndex);
    if (bufferIndex == m_entityBufferCount)
    {
        be_assert(m_entityInfoBuffer[bufferIndex] == 0,
                  "buffer index %d inconsistent" | bufferIndex);
        EntityInfo* buffer = (EntityInfo*)m_entityAllocator.allocate();
        u32 index = 1 + (bufferIndex << 16);
        for (u32 i = 0; i < m_bufferCapacity; ++i)
        {
            buffer[i].next = index + i + 1;
            buffer[i].mask = minitl::bitset<96>();
        }
        buffer[m_bufferCapacity-1].next = (1 + bufferIndex) << 16;

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
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return);
    be_forceuse(c);
    ComponentIndex componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(), return);
    /*ComponentGroup& group = */getComponentGroup(componentIndex);
    be_assert_recover(!info.mask[componentIndex.index],
                      "entity %d already has a component %s" | e.id | componentType->fullname(), return);
    info.mask[componentIndex.index] = true;
}

void EntityStorage::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return);
    ComponentIndex componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(), return);
    be_assert_recover(info.mask[componentIndex.index],
                      "entity %d does not has a component %s" | e.id | componentType->fullname(), return);
    info.mask[componentIndex.index] = false;
}

bool EntityStorage::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    const EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return false);
    ComponentIndex componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage" | componentType->fullname(), return false);
    return info.mask[componentIndex.index];
}

EntityStorage::ComponentGroup& EntityStorage::getComponentGroup(ComponentIndex componentIndex)
{
    be_assert(componentIndex,
              "invalid component index given to getComponentGroup");
    be_assert(componentIndex.group < m_componentGroups.size(),
              "invalid component index given to getComponentGroup");
    return m_componentGroups[componentIndex.group];
}


}}
