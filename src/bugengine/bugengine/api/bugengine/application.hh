/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_APPLICATION_HH_
#define BE_BUGENGINE_APPLICATION_HH_
/**************************************************************************************************/
#include <bugengine/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/loader.hh>
#include <bugengine/scheduler/task/group.hh>
#include <bugengine/scriptengine.hh>

namespace BugEngine {

namespace KernelScheduler {
class IKernelScheduler;
}

namespace World {
class World;
}
class Folder;

class be_api(BUGENGINE) Application : public Resource::ILoader
{
    BE_NOCOPY(Application);

private:
    class WorldResource;
    struct UpdateTask
    {
        ref< Task::ITask >                   task;
        Task::TaskGroup::TaskStartConnection start;
        Task::TaskGroup::TaskEndConnection   end;
    };

private:
    ref< Folder > const                                 m_dataFolder;
    weak< Scheduler >                                   m_scheduler;
    weak< Resource::ResourceManager >                   m_resourceManager;
    Plugin::Context const                               m_pluginContext;
    Plugin::Plugin< KernelScheduler::IKernelScheduler > m_cpuKernelScheduler;
    ref< Task::TaskGroup >                              m_updateTask;
    ref< Task::TaskGroup >                              m_worldTask;
    minitl::vector< UpdateTask >                        m_tasks;
    Task::ITask::CallbackConnection                     m_updateLoop;
    Task::ITask::CallbackConnection                     m_forceContinue;
    Task::ITask::CallbackConnection                     m_worldLoop;
    size_t                                              m_resourceLoadingCount;
    u32                                                 m_worldCount;
    bool                                                m_runLoop;

private:
    void frameUpdate();
    void updateResources();

private:
    virtual void load(weak< const Resource::Description > scene, Resource::Resource & resource)
        override;
    virtual void reload(weak< const Resource::Description > oldScene,
                        weak< const Resource::Description > newScene, Resource::Resource & resource)
        override;
    virtual void unload(Resource::Resource & resource) override;

private:
    void registerInterruptions();
    void unregisterInterruptions();

protected:
    void                   addTask(ref< Task::ITask > task);
    void                   removeTask(ref< Task::ITask > task);
    const Plugin::Context& pluginContext() const
    {
        return m_pluginContext;
    }
    Application(ref< Folder > dataFolder, weak< Resource::ResourceManager > resourceManager,
                weak< Scheduler > scheduler);

    weak< Task::ITask > applicationUpdateTask() const
    {
        return m_updateTask;
    }
    weak< Task::ITask > worlUpdateTask() const
    {
        return m_worldTask;
    }

public:
    virtual ~Application();

    int  run();
    void finish();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
