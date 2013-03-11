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

struct EntityStorage::EntityInfo
{
    union
    {
        u32 next;
        u32 bucket;
    };
    u32 index;
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
    ,   m_entityAllocator(sizeof(EntityInfo)*4*1024*1024)
    ,   m_entityInfoBuffer((EntityInfo*)m_entityAllocator.buffer())
    ,   m_entityCount(0)
    ,   m_entityCapacity(0)
    ,   m_componentTypes(Arena::game())
{
    m_componentTypes.reserve(64);
}

EntityStorage::~EntityStorage()
{
    be_assert(m_entityCount == 0, "%d entities still spawned when deleting world" | m_entityCount);
    m_entityAllocator.setUsage(0);
}

weak<Task::ITask> EntityStorage::initialTask() const
{
    return m_task;
}

void EntityStorage::registerType(raw<const RTTI::Class> componentType)
{
    if (componentType != be_typeid<void>::klass())
    {
        be_assert(indexOf(componentType) == (u32)-1, "component type %s is registered twice" | componentType->fullname());
        m_componentTypes.push_back(componentType);
    }
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

void EntityStorage::start()
{
}

Entity EntityStorage::spawn()
{
    Entity e = { m_freeEntityId };

    if (e.id >= m_entityCapacity)
    {
        m_entityCapacity += 1024;
        m_entityAllocator.setUsage(m_entityCapacity*sizeof(EntityInfo));
        for (u32 i = m_entityCapacity - 1024; i < m_entityCapacity; ++i)
        {
            m_entityInfoBuffer[i].next = i+1;
        }
    }

    be_assert((m_entityInfoBuffer[e.id].index & s_usedBit) == 0, "Entity %s is already in use; entity buffer inconsistent");
    m_freeEntityId = m_entityInfoBuffer[e.id].next;
    m_entityInfoBuffer[e.id].bucket = 0;
    m_entityInfoBuffer[e.id].index = s_usedBit;
    m_entityInfoBuffer[e.id].mask = 0;
    ++ m_entityCount;
    return e;
}

void EntityStorage::unspawn(Entity e)
{
    be_assert_recover(e.id < m_entityCapacity, "Entity has invalid ID %s" | e.id, return);
    be_assert_recover((m_entityInfoBuffer[e.id].index & s_usedBit) != 0, "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return);
    m_entityInfoBuffer[e.id].next = m_freeEntityId;
    m_freeEntityId = e.id;
    // unspawn components
    m_entityInfoBuffer[e.id].index = 0;
    m_entityInfoBuffer[e.id].mask = 0;
    -- m_entityCount;
}

void EntityStorage::addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType)
{
    be_assert_recover(e.id < m_entityCapacity, "Entity has invalid ID %s" | e.id, return);
    be_assert_recover((m_entityInfoBuffer[e.id].index & s_usedBit) != 0, "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return);
    be_forceuse(c);
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1), "component type %s is not a registered component of entoty storage" | componentType->fullname(), return);
    u64 mask = 1;
    mask <<= componentIndex;
    be_assert_recover((m_entityInfoBuffer[e.id].mask & mask) == 0, "entity %d already has a component %s" | e.id | componentType->fullname(), return);
    m_entityInfoBuffer[e.id].mask |= mask;
}

void EntityStorage::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    be_assert_recover(e.id < m_entityCapacity, "Entity has invalid ID %s" | e.id, return);
    be_assert_recover((m_entityInfoBuffer[e.id].index & s_usedBit) != 0, "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return);
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1), "component type %s is not a registered component of entoty storage" | componentType->fullname(), return);
    u64 mask = 1;
    mask <<= componentIndex;
    be_assert_recover((m_entityInfoBuffer[e.id].mask & mask) != 0, "entity %d does not has a component %s" | e.id | componentType->fullname(), return);
    m_entityInfoBuffer[e.id].mask &= ~mask;
}

bool EntityStorage::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    be_assert_recover(e.id < m_entityCapacity, "Entity has invalid ID %s" | e.id, return false);
    be_assert_recover((m_entityInfoBuffer[e.id].index & s_usedBit) != 0, "Entity %s is not currently spawned (maybe already unspawned)" | e.id, return false);
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1), "component type %s is not a registered component of entoty storage" | componentType->fullname(), return false);
    u64 mask = 1;
    mask <<= componentIndex;
    return (m_entityInfoBuffer[e.id].mask & mask) != 0;
}


}}
