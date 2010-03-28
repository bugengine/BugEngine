/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/world.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine
{

be_abstractmetaclass_impl("",World);

enum
{
    WorldUpdateTask_CopyWorld   = 0,
    WorldUpdateTask_UpdateWorld = 1,
    WorldUpdateTask_SwapWorld   = 2,
    WorldUpdateTask_Count
};

static int s_worldIndex = 0;

World::World(float3 worldExtents)
:   m_physicsSystem("physicsBullet")
,   m_audioSystem("audioOpenAL")
,   m_physicsWorld(m_physicsSystem->createWorld(worldExtents))
,   m_audioWorld(m_audioSystem->createWorld())
,   m_callbacks()
,   m_tasks()
,   m_worldIndex(s_worldIndex++)
{
    m_tasks.resize(WorldUpdateTask_Count);

    m_tasks[WorldUpdateTask_CopyWorld] = ref< Task< MethodCaller<World, &World::copyWorld> > >::create("copyWorld", color32(255,0,255),  MethodCaller<World, &World::copyWorld>(this));
    m_tasks[WorldUpdateTask_UpdateWorld] = ref< Task< MethodCaller<World, &World::updateWorld> > >::create("updateWorld", color32(255,0,255),  MethodCaller<World, &World::updateWorld>(this));
    m_tasks[WorldUpdateTask_SwapWorld] = ref< Task< MethodCaller<World, &World::swapWorld> > >::create("swapWorld", color32(255,0,255),  MethodCaller<World, &World::swapWorld>(this));
}

World::~World()
{
}

weak<ITask> World::copyWorldTask()
{
    return m_tasks[WorldUpdateTask_CopyWorld];
}

weak<ITask> World::updateWorldTask()
{
    return m_tasks[WorldUpdateTask_UpdateWorld];
}

weak<ITask> World::swapWorldTask()
{
    return m_tasks[WorldUpdateTask_SwapWorld];
}


void World::copyWorld()
{
}

void World::updateWorld()
{
}

void World::swapWorld()
{
}

}
