/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>
#include    <system/resource/resourcehandle.hh>

namespace BugEngine
{

namespace Graphics
{
class IScene;
}

class Application : public minitl::refcountable
{
    BE_NOCOPY(Application);
private:
    class SceneResource;
private:
    scoped<Scheduler>                               m_scheduler;
    minitl::vector< ref<ITask> >                    m_tasks;
    minitl::vector<TaskGroup::TaskStartConnection>  m_startConnections;
    minitl::vector<TaskGroup::TaskEndConnection>    m_endConnections;
private:
    void frameUpdate();
public:
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(void);

    ResourceHandle addScene(weak<const Graphics::IScene> scene);
    void removeScene(const ResourceHandle& handle);

    weak<const Scheduler> scheduler() const  { return m_scheduler; }
private:
    Application(void);
};

}

/*****************************************************************************/
#endif
