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
,   m_lastEntityId(s_defaultSlot)
,   m_allocator(SystemAllocator::Block64kb, 256)
{
    u8* memory[55];
    memory[0] = (u8*)m_allocator.blockAlloc();
    *memory[0] = 1;
    memory[1] = (u8*)m_allocator.blockAlloc();
    *memory[1] = 1;
    memory[2] = (u8*)m_allocator.blockAlloc();
    *memory[2] = 1;
    memory[3] = (u8*)m_allocator.blockAlloc();
    *memory[3] = 1;
    memory[4] = (u8*)m_allocator.blockAlloc();
    *memory[4] = 1;

    m_allocator.blockFree(memory[0]);
    memory[0] = (u8*)m_allocator.blockAlloc();
    *memory[0] = 1;
    m_allocator.blockFree(memory[2]);
    m_allocator.blockFree(memory[3]);
    m_allocator.blockFree(memory[1]);
    m_allocator.blockFree(memory[0]);
    m_allocator.blockFree(memory[4]);
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

