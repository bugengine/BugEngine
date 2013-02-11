/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <scheduler/task/method.hh>

namespace BugEngine { namespace World
{

struct EntityStorage::EntityInfo
{
    Entity next;
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
    ++ m_entityCount;
    return e;
}

void EntityStorage::unspawn(Entity e)
{
    m_entityInfoBuffer[e.id].next = m_freeEntityId;
    m_freeEntityId = e;
    -- m_entityCount;
}

void EntityStorage::addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType)
{
    be_forceuse(e);
    be_forceuse(c);
    be_forceuse(componentType);
}

void EntityStorage::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    be_forceuse(e);
    be_forceuse(componentType);
}

}}
