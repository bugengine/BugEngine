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

static const Entity s_defaultSlot = { 0, 0 };

World::World()
:   m_task(ref<TaskGroup>::create(taskArena(), "world:update", color32(89, 89, 180)))
,   m_emptyEntityState(scoped<State>::create(gameArena()))
,   m_blockManager(scoped<BlockManager>::create(gameArena()))
,   m_freeEntityId(s_defaultSlot)
,   m_lastEntityId(s_defaultSlot)
,   m_entityBlocks(scoped< Block<EntitySlot, 64> >::create(gameArena(), m_blockManager))
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
    Entity e = m_freeEntityId;

    return e;
}

}}

