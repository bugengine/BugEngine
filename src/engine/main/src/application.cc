/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/application.hh>

#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine
{

Application::Application(int argc, const char *argv[])
:   m_scheduler(scoped<Scheduler>::create())
,   m_tasks()
{
    be_forceuse(argc); be_forceuse(argv);

    ref< TaskGroup > updateTask = ref< TaskGroup >::create("applicationUpdate", color32(255,255,0));
    m_tasks.push_back(updateTask);

    m_tasks.push_back(ref< Task< MethodCaller<Scheduler, &Scheduler::frameUpdate> > >::create("scheduler", color32(255,255,0), MethodCaller<Scheduler, &Scheduler::frameUpdate>(m_scheduler)));
    m_tasks.push_back(ref< Task< ProcedureCaller<&Malloc::frameUpdate> > >::create("memory", color32(255,255,0), ProcedureCaller<&Malloc::frameUpdate>()));
    m_tasks.push_back(ref< Task< MethodCaller<Application, &Application::frameUpdate> > >::create("application", color32(255,255,0), MethodCaller<Application, &Application::frameUpdate>(this)));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(updateTask, m_tasks[1]));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(updateTask, m_tasks[2]));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(updateTask, m_tasks[3]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(updateTask, m_tasks[1]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(updateTask, m_tasks[2]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(updateTask, m_tasks[3]));
}

Application::~Application(void)
{
}

void Application::frameUpdate()
{
    if(m_scene && m_scene->closed())
    {
        setScene(scoped<Graphics::INode>());
    }
}

int Application::run()
{
    m_tasks[0]->run(m_scheduler);
    m_scheduler->wait();
    return 0;
}

void Application::setScene(scoped<Graphics::INode> scene)
{
    if(scene)
    {
        m_startSceneUpdate = ITask::CallbackConnection(m_tasks[0], scene->renderTask()->startCallback());
        m_endSceneUpdate = ITask::CallbackConnection(scene->renderTask(),m_tasks[0]->startCallback());
    }
    else
    {
        m_startSceneUpdate = ITask::CallbackConnection();
        m_endSceneUpdate = ITask::CallbackConnection();
    }
    m_scene = scene;
}

}
