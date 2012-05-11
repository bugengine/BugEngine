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
,   m_freeEntityId(s_defaultSlot)
,   m_allocator16k(SystemAllocator::Block64kb, 2048)
,   m_allocator64k(SystemAllocator::Block64kb, 512)
,   m_entityBuffers(gameArena())
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

    if (e.block >= m_entityBuffers.size())
    {
        be_assert(e.block == m_entityBuffers.size(), "mismatch in the entity buffer!");
        Entity* newBuffer = static_cast<Entity*>(m_allocator64k.blockAlloc());
        size_t entityCount = m_allocator64k.blockSize()/sizeof(Entity);
        for (size_t i = 0; i < entityCount-1; ++i)
        {
            newBuffer[i].block = e.block;
            newBuffer[i].index = i+1;
        }
        newBuffer[entityCount-1].block = e.block+1;
        newBuffer[entityCount-1].index = 0;
    }

    m_freeEntityId = m_entityBuffers[e.block][e.index];
    ++ m_entityCount;
    return e;
}


void World::unspawn(Entity e)
{
    m_entityBuffers[e.block][e.index] = m_freeEntityId;
    m_freeEntityId = e;
    -- m_entityCount;
}
}}

