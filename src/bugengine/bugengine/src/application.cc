/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/stdafx.h>
#include <bugengine/application.hh>

#include <bugengine/core/timer.hh>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/loader.hh>
#include <bugengine/scheduler/task/group.hh>
#include <bugengine/scheduler/task/method.hh>
#include <bugengine/world/world.script.hh>

namespace BugEngine {

class Application::WorldResource : public minitl::refcountable
{
    BE_NOCOPY(WorldResource);

private:
    Task::TaskGroup::TaskStartConnection m_startSceneUpdate;
    Task::TaskGroup::TaskEndConnection   m_endSceneUpdate;

public:
    WorldResource(weak< const World::World > world, weak< Task::TaskGroup > task);
    ~WorldResource();
};

Application::WorldResource::WorldResource(weak< const World::World > world,
                                          weak< Task::TaskGroup >    task)
    : m_startSceneUpdate(task, world->updateWorldTask())
    , m_endSceneUpdate(task, world->updateWorldTask())
{
}

Application::WorldResource::~WorldResource()
{
}

Application::Application(ref< Folder >                     dataFolder,
                         weak< Resource::ResourceManager > resourceManager,
                         weak< Scheduler >                 scheduler)
    : ILoader()
    , m_dataFolder(dataFolder)
    , m_scheduler(scheduler)
    , m_resourceManager(resourceManager)
    , m_pluginContext(resourceManager, m_dataFolder, m_scheduler)
    , m_cpuKernelScheduler("plugin.compute.cpu", m_pluginContext)
    , m_updateTask(ref< Task::TaskGroup >::create(Arena::task(), "applicationUpdate",
                                                  Colors::Yellow::Yellow))
    , m_worldTask(
          ref< Task::TaskGroup >::create(Arena::task(), "worldUpdate", Colors::Yellow::Yellow))
    , m_tasks(Arena::task())
    , m_updateLoop(m_updateTask, m_worldTask->startCallback())
    , m_forceContinue()
    , m_worldLoop(m_worldTask, m_updateTask->startCallback())
    , m_resourceLoadingCount(0)
    , m_worldCount(0)
    , m_runLoop(true)
{
    m_resourceManager->attach< World::World >(this);
    addTask(ref< Task::Task< Task::MethodCaller< Application, &Application::updateResources > > >::
                create(Arena::task(), "resource", Colors::Green::Green,
                       Task::MethodCaller< Application, &Application::updateResources >(this)));
    addTask(
        ref< Task::Task< Task::MethodCaller< Application, &Application::frameUpdate > > >::create(
            Arena::task(), "update", Colors::Green::Green,
            Task::MethodCaller< Application, &Application::frameUpdate >(this)));
    registerInterruptions();
}

Application::~Application(void)
{
    unregisterInterruptions();
    m_resourceManager->detach< World::World >(this);
}

void Application::addTask(ref< Task::ITask > task)
{
    UpdateTask t;
    t.task  = task;
    t.start = Task::TaskGroup::TaskStartConnection(m_updateTask, task);
    t.end   = Task::TaskGroup::TaskEndConnection(m_updateTask, task);
    m_tasks.push_back(t);
}

void Application::removeTask(ref< Task::ITask > task)
{
    for(size_t i = 0; i < m_tasks.size(); ++i)
    {
        if(m_tasks[i].task == task)
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

void Application::load(weak< const Resource::Description > world, Resource::Resource& resource)
{
    if(m_worldCount == 0)
    {
        m_updateLoop = Task::ITask::CallbackConnection(m_updateTask, m_worldTask->startCallback());
    }
    m_worldCount++;
    resource.setRefHandle(ref< WorldResource >::create(
        Arena::resource(), be_checked_cast< const World::World >(world), m_worldTask));
}

void Application::reload(weak< const Resource::Description > /*oldWorld*/,
                         weak< const Resource::Description > newWorld, Resource::Resource& resource)
{
    unload(resource);
    load(newWorld, resource);
}

void Application::unload(Resource::Resource& resource)
{
    m_worldCount--;
    if(m_worldCount == 0)
    {
        be_info("Last World destroyed - stopping application");
        m_worldLoop = Task::ITask::CallbackConnection();
    }
    resource.clearRefHandle();
}

void Application::updateResources()
{
    size_t resourceCount = m_resourceManager->updateTickets();
    if(resourceCount == 0 && m_resourceLoadingCount != 0)
    {
        m_forceContinue = Task::ITask::CallbackConnection();
    }
    else if(resourceCount != 0 && m_resourceLoadingCount == 0)
    {
        m_forceContinue
            = Task::ITask::CallbackConnection(m_worldTask, m_updateTask->startCallback());
    }
    m_resourceLoadingCount = resourceCount;
    if(!m_runLoop)
    {
        m_worldLoop = Task::ITask::CallbackConnection();
    }
}

void Application::frameUpdate()
{
    static int   frames     = 0;
    static int   frameCount = 100;
    static float now        = Timer::now();
    if(++frames % frameCount == 0)
    {
        float time = Timer::now();
        float t    = (time - now) / float(frameCount);
        if(t > 10.0f)
        {
            be_info("Average frame time (%d frames): %d milliseconds" | frameCount | (int)t);
            frameCount = 20;
        }
        else
        {
            t = 1000.0f * t;
            if(t > 10.0f)
            {
                be_info("Average frame time (%d frames): %d microseconds" | frameCount | (int)t);
                frameCount = 5000;
            }
            else
            {
                be_info("Average frame time (%d frames): %d nanoseconds" | frameCount
                        | (int)(t * 1000.0f));
                frameCount = 200000;
            }
        }
        now = time;
    }
}

void Application::finish()
{
    m_runLoop = false;
}

}  // namespace BugEngine
