/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/world.script.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

#include    <state.hh>

BE_REGISTER_NAMESPACE_2_NAMED(game, BugEngine, World);

namespace BugEngine { namespace World
{

World::World()
:   m_task(ref<TaskGroup>::create(taskArena(), "world:update", color32(89, 89, 180)))
,   m_emptyEntityState(scoped<State>::create(gameArena()))
,   m_freeEntityId(0)
,   m_entityBlocks(gameArena())
{
}

World::~World()
{
}

weak<ITask> World::updateWorldTask() const
{
    return m_task;
}

Entity World::spawn()
{
    u32 newEntity = m_freeEntityId;
    u32 newEntityStorage = newEntity >> 22;
    u32 newEntityIndex = newEntity & ((1<<22) - 1);
    if (newEntityStorage > m_entityBlocks.size())
    {
        m_entityBlocks.resize(m_entityBlocks.size() + 4);
    }
    Entity e = { m_freeEntityId };
    m_freeEntityId = (*m_entityBlocks[newEntityStorage])[newEntityIndex].nextEntity;
    return e;
}

}}

