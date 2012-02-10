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
    TaskGroup::TaskStartConnection  m_startSceneUpdate;
    TaskGroup::TaskEndConnection    m_endSceneUpdate;
public:
    WorldResource(weak<const World::World> world, weak<TaskGroup> task);
    ~WorldResource();
};

Application::WorldResource::WorldResource(weak<const World::World> world, weak<TaskGroup> task)
    :   m_startSceneUpdate(task, world->updateWorldTask())
    ,   m_endSceneUpdate(task, world->updateWorldTask())
{
}

Application::WorldResource::~WorldResource()
{
}

Application::Application()
:   IResourceLoader()
,   m_scheduler(scoped<Scheduler>::create(taskArena()))
,   m_updateTask(ref< TaskGroup >::create(taskArena(), "applicationUpdate", color32(255,255,0)))
,   m_worldTask(ref< TaskGroup >::create(taskArena(), "worldUpdate", color32(255,255,0)))
,   m_tasks(taskArena())
,   m_updateLoop(m_updateTask, m_worldTask->startCallback())
,   m_worldLoop(m_worldTask, m_updateTask->startCallback())
{
    addTask(ref< Task< MethodCaller<Scheduler, &Scheduler::frameUpdate> > >::create(taskArena(), "scheduler", color32(255,255,0), MethodCaller<Scheduler, &Scheduler::frameUpdate>(m_scheduler)));
    //m_updateLoop = ITask::CallbackConnection();
}

Application::~Application(void)
{
}

void Application::addTask(ref<ITask> task)
{
    UpdateTask t;
    t.task = task;
    t.start = TaskGroup::TaskStartConnection(m_updateTask, task);
    t.end = TaskGroup::TaskEndConnection(m_updateTask, task);
    m_tasks.push_back(t);
}

void Application::removeTask(ref<ITask> task)
{
    for (size_t i = 0; i < m_tasks.size(); ++i)
    {
        if (m_tasks[i].task == task)
        {
            minitl::swap(m_tasks[i], m_tasks.back());
            m_tasks.pop_back();
        }
    }
}

int Application::run()
{
    m_updateTask->run(m_scheduler);
    m_scheduler->mainThreadJoin();
    return 0;
}

ResourceHandle Application::load(weak<const Resource> world)
{
    ResourceHandle handle;
    handle.handle = ref<WorldResource>::create(taskArena(), be_checked_cast<const World::World>(world), m_worldTask);
    return handle;
}

void Application::unload(const ResourceHandle& /*handle*/)
{
}

}
