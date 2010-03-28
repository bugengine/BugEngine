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
,   m_callbacks()
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

    m_startUpdate = ref<ITask::ChainCallback>::create();
    m_callbacks.push_back(m_startUpdate);
    m_startUpdate->makeStart(updateTask);

    m_onUpdate = ref<ITask::ChainCallback>::create();
    m_callbacks.push_back(m_onUpdate);
    updateTask->addCallback(m_onUpdate);
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

void Application::addScene(ref<Graphics::Scene> scene, ref<Graphics::RenderTarget> target)
{
    //m_views.push_back(
}

void Application::addWorld(ref<World> world)
{
    m_onUpdate->makeStart(world->copyWorldTask());

    ref<ITask::ChainCallback> callback = ref<ITask::ChainCallback>::create();
    m_callbacks.push_back(callback);
    callback->makeStart(world->updateWorldTask());
    world->copyWorldTask()->addCallback(callback);

    callback = ref<ITask::ChainCallback>::create();
    m_callbacks.push_back(callback);
    callback->makeStart(world->swapWorldTask());
    world->updateWorldTask()->addCallback(callback);

    world->swapWorldTask()->addCallback(m_startUpdate);

    m_worlds.push_back(world);
}


}
