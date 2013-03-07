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
        Entity next;
        u32 bucket;
    };
    u32 index;
    u64 mask;
};

static const Entity s_defaultSlot = { 0 };

EntityStorage::EntityStorage()
    :   m_task(scoped< Task::Task< Task::MethodCaller<EntityStorage, &EntityStorage::start> > >::create(
                    Arena::task(),
                    "start",
                    Colors::Green::Green,
                    Task::MethodCaller<EntityStorage, &EntityStorage::start>(this)))
    ,   m_freeEntityId(s_defaultSlot)
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
    Entity e = m_freeEntityId;

    if (e.id >= m_entityCapacity)
    {
        m_entityCapacity += 1024;
        m_entityAllocator.setUsage(m_entityCapacity*sizeof(EntityInfo));
        for (u32 i = m_entityCapacity - 1024; i < m_entityCapacity; ++i)
        {
            m_entityInfoBuffer[i].next.id = i+1;
        }
    }

    m_freeEntityId = m_entityInfoBuffer[e.id].next;
    m_entityInfoBuffer[e.id].next.id = 0;
    m_entityInfoBuffer[e.id].index = 0;
    m_entityInfoBuffer[e.id].mask = 0;
    ++ m_entityCount;
    return e;
}

void EntityStorage::unspawn(Entity e)
{
    m_entityInfoBuffer[e.id].next = m_freeEntityId;
    m_freeEntityId = e;
    // unspawn components
    m_entityInfoBuffer[e.id].index = 0;
    m_entityInfoBuffer[e.id].mask = 0;
    -- m_entityCount;
}

void EntityStorage::addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType)
{
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
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1), "component type %s is not a registered component of entoty storage" | componentType->fullname(), return);
    u64 mask = 1;
    mask <<= componentIndex;
    be_assert_recover((m_entityInfoBuffer[e.id].mask & mask) != 0, "entity %d does not has a component %s" | e.id | componentType->fullname(), return);
    m_entityInfoBuffer[e.id].mask &= ~mask;
}

bool EntityStorage::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    u32 componentIndex = indexOf(componentType);
    be_assert_recover(componentIndex != u32(-1), "component type %s is not a registered component of entoty storage" | componentType->fullname(), return false);
    u64 mask = 1;
    mask <<= componentIndex;
    return (m_entityInfoBuffer[e.id].mask & mask) != 0;
}


}}
