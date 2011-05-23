/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>

namespace BugEngine
{

class WorldScene;

class Application : public minitl::refcountable
{
private:
    scoped<Scheduler>                               m_scheduler;
    ref<WorldScene>                                 m_scene;
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

    void setScene(scoped<WorldScene> scene);

    weak<const Scheduler> scheduler() const  { return m_scheduler; }
private:
    Application(void);
    Application(const Application&);
    const Application& operator=(const Application&);
};

}

/*****************************************************************************/
#endif
