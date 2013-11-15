/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <scheduler/task/method.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/typeinfo.hh>

namespace BugEngine { namespace World
{

EntityStorage::Bucket::Bucket()
:   acceptMask(0)
,   rejectMask(0)
,   componentCounts(0)
{
}

EntityStorage::Bucket::Bucket(u32 componentCount, u64 acceptMask, u64 rejectMask)
:   acceptMask(acceptMask)
,   rejectMask(rejectMask)
,   componentCounts()
{
    be_forceuse(componentCount);
}

EntityStorage::Bucket::~Bucket()
{
}

EntityStorage::ComponentGroup::ComponentGroup(u64 mask)
:   componentMask(mask)
,   buckets(Arena::game(), 1)
{
    buckets[0] = Bucket(1, mask, 0);
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
    u32 bucket;
    u64 mask;
};

static const u32 s_usedBit = 0x80000000;
static const u32 s_indexMask = 0x7FFFFFFF;

EntityStorage::EntityStorage()
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
    ,   m_componentTypes(Arena::game())
    ,   m_componentGroups(Arena::game())
{
    m_entityInfoBuffer[0] = 0;
    m_componentTypes.reserve(64);
}

EntityStorage::~EntityStorage()
{
    be_assert(m_entityCount == 0,
              "%d entities still spawned when deleting world" | m_entityCount);
    for (EntityInfo** buffer = m_entityInfoBuffer; *buffer != 0; ++buffer)
    {
        m_entityAllocator.free(*buffer);
    }
    m_entityAllocator.free(m_entityInfoBuffer);
}

weak<Task::ITask> EntityStorage::initialTask() const
{
    return m_task;
}

void EntityStorage::registerType(raw<const RTTI::Class> componentType, u32 maximumCount)
{
    be_assert(indexOf(componentType) == (u32)-1,
              "component type %s is registered twice" | componentType->fullname());
    be_info("component %s: reserving space for %d" | componentType->name | maximumCount);
    m_componentTypes.push_back(componentType);
    u32 index = be_checked_numcast<u32>(m_componentTypes.size()) - 1;
    u64 mask = u64(1) << index;
    m_componentGroups.push_back(ComponentGroup(mask));
}

u32 EntityStorage::indexOf(raw<const RTTI::Class> componentType) const
{
    for (u32 i = 0; i < m_componentTypes.size(); ++i)
    {
        if (m_componentTypes[i] == componentType)
        {
            return i;
        }
    }
    return (u32) -1;
}

void EntityStorage::finalize()
{
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
            buffer[i].bucket = 0;
            buffer[i].mask = 0;
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
    entityInfo.bucket = 0;
    entityInfo.mask = 0;
    ++ m_entityCount;
    return e;
}

void EntityStorage::unspawn(Entity e)
{
    EntityInfo& info = getEntityInfo(e);
    // unspawn components
    info.next = m_freeEntityId;
    info.bucket = 0;
    info.mask = 0;
    m_freeEntityId = e.id;
    -- m_entityCount;
}

void EntityStorage::addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType)
{
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return);
    be_forceuse(c);
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1),
                      "component type %s is not a registered component of entoty storage" | componentType->fullname(), return);
    /*ComponentGroup& group = */getComponentGroup(componentIndex);
    u64 mask = u64(1) << componentIndex;
    be_assert_recover((info.mask & mask) == 0,
                      "entity %d already has a component %s" | e.id | componentType->fullname(), return);
    info.mask |= mask;
}

void EntityStorage::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return);
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1),
                      "component type %s is not a registered component of entoty storage" | componentType->fullname(), return);
    u64 mask = 1;
    mask <<= componentIndex;
    be_assert_recover((info.mask & mask) != 0,
                      "entity %d does not has a component %s" | e.id | componentType->fullname(), return);
    info.mask &= ~mask;
}

bool EntityStorage::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    const EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return false);
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1),
                      "component type %s is not a registered component of entoty storage" | componentType->fullname(), return false);
    u64 mask = 1;
    mask <<= componentIndex;
    return (info.mask & mask) != 0;
}

EntityStorage::ComponentGroup& EntityStorage::getComponentGroup(u32 componentIndex)
{
    u64 mask = u64(1) << componentIndex;
    for (minitl::vector<ComponentGroup>::iterator it = m_componentGroups.begin();
         it != m_componentGroups.end();
         ++it)
    {
        if ((it->componentMask & mask) == mask)
        {
            return *it;
        }
    }
    be_notreached();
    return m_componentGroups.front();
}


}}
