/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/application.hh>

#include    <rtti/namespace.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine
{

be_abstractmetaclass_impl("",Application);


Application::Application(int argc, const char *argv[])
:   Object()
,   m_scheduler(scoped<Scheduler>::create())
,   m_tasks()
{
    UNUSED(argc); UNUSED(argv);

    ref< TaskGroup > updateTask = ref< TaskGroup >::create("application", color32(255,255,0));
    m_tasks.push_back(updateTask);

    m_tasks.push_back(ref< Task< MethodCaller<Scheduler, &Scheduler::frameUpdate> > >::create("scheduler", color32(255,255,0), MethodCaller<Scheduler, &Scheduler::frameUpdate>(m_scheduler)));
    m_tasks.push_back(ref< Task< ProcedureCaller<&Malloc::frameUpdate> > >::create("memory", color32(255,255,0), ProcedureCaller<&Malloc::frameUpdate>()));
    updateTask->addStartTask(m_tasks[1]);
    updateTask->addStartTask(m_tasks[2]);
    updateTask->addEndTask(m_tasks[1]);
    updateTask->addEndTask(m_tasks[2]);
}

Application::~Application(void)
{
}

int Application::run()
{
    m_tasks[0]->run(m_scheduler);
    m_scheduler->wait();
    return 0;
}

ref<Graphics::RenderTarget> Application::createWindow(Graphics::WindowFlags f)
{
    return ref<Graphics::RenderTarget>(); //m_display->createWindow(f);
}

void Application::addScene(ref<Graphics::Scene> scene, ref<Graphics::RenderTarget> target)
{
    //m_views.push_back(
}

void Application::addWorld(ref<World> world)
{
    /*world->getStart()->connectFrom(m_updateInputTask);
    world->getStart()->connectFrom(m_updateMemoryTask);
    world->getStart()->connectFrom(m_updateSchedulerTask);
    world->getEnd()->connectTo(m_updateInputTask);
    world->getEnd()->connectTo(m_updateMemoryTask);
    world->getEnd()->connectTo(m_updateSchedulerTask);
    m_worlds.push_back(world);*/
}


}
