/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_APPLICATION_HH_
#define BE_BUGENGINE_APPLICATION_HH_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>
#include    <system/resource/resourcehandle.hh>
#include    <bugengine/scriptengine.hh>
#include    <system/plugin.hh>

namespace BugEngine
{

class IKernelScheduler;
namespace World
{
class World;
}
class Folder;

class be_api(BUGENGINE) Application : public IResourceLoader
{
    BE_NOCOPY(Application);
private:
    class WorldResource;
    struct UpdateTask
    {
        ref<ITask>                      task;
        TaskGroup::TaskStartConnection  start;
        TaskGroup::TaskEndConnection    end;
    };
private:
    ref<Folder> const               m_dataFolder;
    scoped<ResourceManager> const   m_resourceManager;
    weak<Scheduler>                 m_scheduler;
    PluginContext const             m_pluginContext;
    Plugin<IKernelScheduler>        m_cpuKernelScheduler;
    ref<TaskGroup>                  m_updateTask;
    ref<TaskGroup>                  m_worldTask;
    minitl::vector< UpdateTask >    m_tasks;
    ITask::CallbackConnection       m_updateLoop;
    ITask::CallbackConnection       m_forceContinue;
    ITask::CallbackConnection       m_worldLoop;
    size_t                          m_resourceLoadingCount;
private:
    void frameUpdate();
    void updateResources();
private:
    virtual ResourceHandle load(weak<const Resource> scene) override;
    virtual void unload(const ResourceHandle& handle) override;
protected:
    void addTask(ref<ITask> task);
    void removeTask(ref<ITask> task);
    const PluginContext& pluginContext() const { return m_pluginContext; }
    Application(ref<Folder> dataFolder, weak<Scheduler> scheduler);
public:
    virtual ~Application();

    int run();
};

}

/*****************************************************************************/
#endif
