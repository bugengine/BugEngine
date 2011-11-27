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
,   m_packageLoader("packagebuilder")
,   m_updateTask(ref< TaskGroup >::create(taskArena(), "applicationUpdate", color32(255,255,0)))
,   m_tasks(taskArena())
,   m_startConnections(taskArena())
,   m_endConnections(taskArena())
,   m_updateLoop(m_updateTask, m_updateTask->startCallback())
{
    be_forceuse(argc); be_forceuse(argv);

    m_tasks.push_back(ref< Task< MethodCaller<Scheduler, &Scheduler::frameUpdate> > >::create(taskArena(), "scheduler", color32(255,255,0), MethodCaller<Scheduler, &Scheduler::frameUpdate>(m_scheduler)));
    m_tasks.push_back(ref< Task< MethodCaller<Application, &Application::updatePackage> > >::create(taskArena(), "package", color32(255,255,0), MethodCaller<Application, &Application::updatePackage>(this)));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(m_updateTask, m_tasks[0]));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(m_updateTask, m_tasks[1]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(m_updateTask, m_tasks[0]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(m_updateTask, m_tasks[1]));

    ResourceLoaders::attach< World, Application >(this, &Application::addWorld, &Application::removeWorld);
    //m_updateLoop = ITask::CallbackConnection();
}

Application::~Application(void)
{
    ResourceLoaders::detach< World, Application >(this);
}

void Application::updatePackage()
{
    if (m_packageLoader)
    {
        m_packageLoader->update();
    }
}

int Application::run(weak<const File> package)
{
    if (package)
    {
        m_packageLoader->loadFile(package, tempArena());

        m_updateTask->run(m_scheduler);
        m_scheduler->mainThreadJoin();
        return 0;
    }
    else
    {
        be_error("could not open main package file");
        return 1;
    }
}

ResourceHandle Application::addWorld(weak<const World> world)
{
    ResourceHandle handle;
    handle.handle = ref<WorldResource>::create(taskArena(), world, m_updateTask);
    return handle;
}

void Application::removeWorld(const ResourceHandle& /*handle*/)
{
}

}
