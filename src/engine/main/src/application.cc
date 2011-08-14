/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/application.hh>

#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>
#include    <system/resource/resourceloader.hh>

#include    <world/world.script.hh>


namespace BugEngine
{

class Application::WorldResource : public minitl::refcountable
{
    BE_NOCOPY(WorldResource);
private:
    ITask::CallbackConnection                       m_startSceneUpdate;
    ITask::CallbackConnection                       m_endSceneUpdate;
public:
    WorldResource(weak<const World> world, weak<ITask> task);
    ~WorldResource();
};

Application::WorldResource::WorldResource(weak<const World> world, weak<ITask> task)
    :   m_startSceneUpdate(task, world->updateWorldTask()->startCallback())
    ,   m_endSceneUpdate(world->updateWorldTask(), task->startCallback())
{
}

Application::WorldResource::~WorldResource()
{
}

Application::Application(int argc, const char *argv[])
:   m_scheduler(scoped<Scheduler>::create(taskArena()))
,   m_tasks(taskArena())
,   m_startConnections(taskArena())
,   m_endConnections(taskArena())
{
    be_forceuse(argc); be_forceuse(argv);

    ref< TaskGroup > updateTask = ref< TaskGroup >::create(taskArena(), "applicationUpdate", color32(255,255,0));
    m_tasks.push_back(updateTask);

    m_tasks.push_back(ref< Task< MethodCaller<Scheduler, &Scheduler::frameUpdate> > >::create(taskArena(), "scheduler", color32(255,255,0), MethodCaller<Scheduler, &Scheduler::frameUpdate>(m_scheduler)));
    //m_tasks.push_back(ref< Task< ProcedureCaller<&Malloc::frameUpdate> > >::create(taskArena(), "memory", color32(255,255,0), ProcedureCaller<&Malloc::frameUpdate>()));
    m_tasks.push_back(ref< Task< MethodCaller<Application, &Application::frameUpdate> > >::create(taskArena(), "application", color32(255,255,0), MethodCaller<Application, &Application::frameUpdate>(this)));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(updateTask, m_tasks[1]));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(updateTask, m_tasks[2]));
    //m_startConnections.push_back(TaskGroup::TaskStartConnection(updateTask, m_tasks[3]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(updateTask, m_tasks[1]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(updateTask, m_tasks[2]));
    //m_endConnections.push_back(TaskGroup::TaskEndConnection(updateTask, m_tasks[3]));

    ResourceLoaders::attach< World, Application >(this, &Application::addWorld, &Application::removeWorld);
}

Application::~Application(void)
{
    ResourceLoaders::detach< World, Application >(this);
}

void Application::frameUpdate()
{
}

int Application::run()
{
    m_tasks[0]->run(m_scheduler);
    m_scheduler->mainThreadJoin();
    return 0;
}

ResourceHandle Application::addWorld(weak<const World> world)
{
    ResourceHandle handle;
    handle.handle = ref<WorldResource>::create(taskArena(), world, m_tasks[0]);
    return handle;
}

void Application::removeWorld(const ResourceHandle& /*handle*/)
{
}

}
