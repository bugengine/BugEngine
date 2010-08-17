/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/world.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine
{

enum
{
    WorldUpdateTask_CopyWorld   = 0,
    WorldUpdateTask_UpdateWorld = 1,
    WorldUpdateTask_Count
};

World::World(istring physics, istring audio, float3 worldExtents)
:   m_physicsSystem(physics, worldExtents)
,   m_audioSystem(audio)
,   m_tasks()
,   m_callbacks()
{
    m_tasks.resize(WorldUpdateTask_Count);

    m_tasks[WorldUpdateTask_CopyWorld] = ref< Task< MethodCaller<World, &World::copyWorld> > >::create("copyWorld", color32(255,0,255),  MethodCaller<World, &World::copyWorld>(this), Scheduler::High);
    m_tasks[WorldUpdateTask_UpdateWorld] = ref< Task< MethodCaller<World, &World::updateWorld> > >::create("updateWorld", color32(255,0,255),  MethodCaller<World, &World::updateWorld>(this));
    m_callbacks.push_back(ITask::CallbackConnection(m_tasks[WorldUpdateTask_CopyWorld], m_tasks[WorldUpdateTask_UpdateWorld]->startCallback()));
    m_callbacks.push_back(ITask::CallbackConnection(m_tasks[WorldUpdateTask_UpdateWorld], m_tasks[WorldUpdateTask_CopyWorld]->startCallback(), ITask::ICallback::Completed));
}

World::~World()
{
}

weak<ITask> World::updateWorldTask()
{
    return m_tasks[WorldUpdateTask_CopyWorld];
}

void World::copyWorld()
{
}

void World::updateWorld()
{
}

}
