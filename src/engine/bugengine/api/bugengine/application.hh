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

namespace World
{
class World;
}

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
    scoped<Scheduler>                               m_scheduler;
    ref<TaskGroup>                                  m_updateTask;
    ref<TaskGroup>                                  m_worldTask;
    minitl::vector< UpdateTask >                    m_tasks;
    ITask::CallbackConnection                       m_updateLoop;
    ITask::CallbackConnection                       m_worldLoop;
protected:
    void addTask(ref<ITask> task);
    void removeTask(ref<ITask> task);
    Application();
public:
    virtual ~Application(void);

    int run();
private:
    virtual ResourceHandle load(weak<const Resource> scene) override;
    virtual void unload(const ResourceHandle& handle) override;
};

}

/*****************************************************************************/
#endif
