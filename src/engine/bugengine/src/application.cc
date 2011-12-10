/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>

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

Application::Application()
:   m_scheduler(scoped<Scheduler>::create(taskArena()))
,   m_updateTask(ref< TaskGroup >::create(taskArena(), "applicationUpdate", color32(255,255,0)))
,   m_tasks(taskArena())
,   m_startConnections(taskArena())
,   m_endConnections(taskArena())
,   m_updateLoop(m_updateTask, m_updateTask->startCallback())
{
    m_tasks.push_back(ref< Task< MethodCaller<Scheduler, &Scheduler::frameUpdate> > >::create(taskArena(), "scheduler", color32(255,255,0), MethodCaller<Scheduler, &Scheduler::frameUpdate>(m_scheduler)));
    m_startConnections.push_back(TaskGroup::TaskStartConnection(m_updateTask, m_tasks[0]));
    m_endConnections.push_back(TaskGroup::TaskEndConnection(m_updateTask, m_tasks[0]));

    ResourceLoaders::attach< World, Application >(this, &Application::addWorld, &Application::removeWorld);
    m_updateLoop = ITask::CallbackConnection();
}

Application::~Application(void)
{
    ResourceLoaders::detach< World, Application >(this);
}

int Application::run()
{
    m_updateTask->run(m_scheduler);
    m_scheduler->mainThreadJoin();
    return 0;
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
