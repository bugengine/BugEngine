/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>
#include    <system/resource/resourcehandle.hh>
#include    <main/scripting.hh>
#include    <system/plugin.hh>

namespace BugEngine
{

class World;

class Application : public minitl::refcountable
{
    BE_NOCOPY(Application);
private:
    class WorldResource;
private:
    scoped<Scheduler>                               m_scheduler;
    Plugin<IScriptEngine>                           m_packageLoader;
    minitl::vector< ref<ITask> >                    m_tasks;
    minitl::vector<TaskGroup::TaskStartConnection>  m_startConnections;
    minitl::vector<TaskGroup::TaskEndConnection>    m_endConnections;
private:
    void updatePackage();
public:
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(weak<const File> package);

    ResourceHandle addWorld(weak<const World> scene);
    void removeWorld(const ResourceHandle& handle);

    weak<const Scheduler> scheduler() const  { return m_scheduler; }
private:
    Application(void);
};

}

/*****************************************************************************/
#endif
