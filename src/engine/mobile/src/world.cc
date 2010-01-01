/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/world.hh>

#include    <system/scheduler/function.hh>


namespace BugEngine
{

be_abstractmetaclass_impl("",World);

World::World(float3 worldExtents, weak< BaseTask::Callback > endJob)
:   m_physicsSystem("physicsBullet")
,   m_audioSystem("audioOpenAL")
,   m_start(ref< BaseTask::Callback >::create())
,   m_startSwap(ref< BaseTask::Callback >::create())
,   m_end(endJob)
,   m_graphicsWorld(ref<Graphics::World>::create(m_startSwap))
,   m_physicsWorld(m_physicsSystem->createWorld(worldExtents, m_startSwap))
,   m_soundWorld(m_audioSystem->createWorld(m_startSwap))
{
    m_tasks.push_back(ref< Task< FunctionBody <World, &World::copyWorld> > >::create("copyWorld", color32(12, 255, 12), FunctionBody <World, &World::copyWorld>(this)));
    m_tasks.push_back(ref< Task< FunctionBody <World, &World::swapWorld> > >::create("swapWorld", color32(12, 255, 12), FunctionBody <World, &World::swapWorld>(this)));

    m_start->connectTo(m_tasks[0]);
    m_end->connectFrom(m_tasks[1]);

    m_physicsWorld->getStart()->connectFrom(m_tasks[0]);
    m_graphicsWorld->getStart()->connectFrom(m_tasks[0]);
    m_soundWorld->getStart()->connectFrom(m_tasks[0]);
    m_startSwap->connectTo(m_tasks[1]);
}

World::~World()
{
}

void World::createView(Graphics::WindowFlags f, ref<Graphics::Scene> scene)
{
    m_graphicsWorld->createWindow(f, scene);
}

void World::copyWorld()
{
}

void World::swapWorld()
{
}

void World::run(weak<Scheduler> scheduler)
{
    m_tasks[0]->run(scheduler);
}

}
