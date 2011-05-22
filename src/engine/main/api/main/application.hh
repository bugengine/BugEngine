/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <mobile/world.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine
{

class Application : public minitl::refcountable
{
private:
    scoped<Scheduler>                               m_scheduler;
    ref<World>                                      m_world;
    ITask::CallbackConnection                       m_startSceneUpdate;
    ITask::CallbackConnection                       m_endSceneUpdate;
    minitl::vector< ref<ITask> >                    m_tasks;
    minitl::vector<TaskGroup::TaskStartConnection>  m_startConnections;
    minitl::vector<TaskGroup::TaskEndConnection>    m_endConnections;
private:
    void frameUpdate();
public:
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(void);

    void setScene(scoped<World> world);

    weak<const Scheduler> scheduler() const  { return m_scheduler; }
private:
    Application(void);
    Application(const Application&);
    const Application& operator=(const Application&);
};

}

/*****************************************************************************/
#endif
