/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>

#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>
#include    <system/resource/resourceloader.hh>
#include    <system/file/folder.script.hh>
#include    <system/plugin.hh>
#include    <core/timer.hh>

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

Application::Application(ref<Folder> dataFolder)
:   IResourceLoader()
,   m_dataFolder(dataFolder)
,   m_resourceManager(scoped<ResourceManager>::create(Arena::game()))
,   m_pluginContext(m_resourceManager, m_dataFolder)
,   m_scheduler(scoped<Scheduler>::create(Arena::task(), m_pluginContext))
,   m_updateTask(ref< TaskGroup >::create(Arena::task(), "applicationUpdate", color32(255,255,0)))
,   m_worldTask(ref< TaskGroup >::create(Arena::task(), "worldUpdate", color32(255,255,0)))
,   m_tasks(Arena::task())
,   m_updateLoop(m_updateTask, m_worldTask->startCallback())
,   m_worldLoop(m_worldTask, m_updateTask->startCallback())
{
    m_resourceManager->attach<World::World>(this);
    addTask(ref< Task< MethodCaller<ResourceManager, &ResourceManager::updateTickets> > >::create(Arena::task(), "resource", color32(0,255,0), MethodCaller<ResourceManager, &ResourceManager::updateTickets>(m_resourceManager)));
    addTask(ref< Task< MethodCaller<Application, &Application::frameUpdate> > >::create(Arena::task(), "update", color32(0,255,0), MethodCaller<Application, &Application::frameUpdate>(this)));
    //m_updateLoop = ITask::CallbackConnection();

}

Application::~Application(void)
{
    m_resourceManager->detach<World::World>(this);
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
    m_updateTask->schedule(m_scheduler);
    m_scheduler->mainThreadJoin();
    return 0;
}

ResourceHandle Application::load(weak<const Resource> world)
{
    ResourceHandle handle;
    handle.handle = ref<WorldResource>::create(Arena::resource(), be_checked_cast<const World::World>(world), m_worldTask);
    return handle;
}

void Application::unload(const ResourceHandle& /*handle*/)
{
}

void Application::frameUpdate()
{
    static int frames = 0;
    static int frameCount = 100;
    static float now = Timer::now();
    if (++frames%frameCount == 0)
    {
        float time = Timer::now();
        float t = (time-now)/float(frameCount);
        if (t > 10.0f)
        {
            be_info("Average frame time: %d milliseconds" | (int)t);
            frameCount = 20;
        }
        else
        {
            t = 1000.0f*t;
            if (t > 10.0f)
            {
                be_info("Average frame time: %d microseconds" | (int)t);
                frameCount = 5000;
            }
            else
            {
                be_info("Average frame time: %d nanoseconds" | (int)(t*1000.0f));
                frameCount = 10000;
            }
        }
        now = time;
    }
}

}
