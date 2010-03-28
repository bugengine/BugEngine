/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <rtti/helper.hh>

#include    <mobile/world.hh>
#include    <graphics/scene/scene.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/renderer/renderbackend.hh>

namespace BugEngine
{

class Application : public Object
{
private:
    class UpdateInput;
    class UpdateScheduler;
    class UpdateMemory;
private:
    scoped<Scheduler>                               m_scheduler;
    minitl::vector< ref<World> >                    m_worlds;
    minitl::vector< ref<ITask> >                    m_tasks;
    ref< ITask::ChainCallback >                     m_startUpdate;
    ref< ITask::ChainCallback >                     m_onUpdate;
    minitl::vector< ref< ITask::ChainCallback > >   m_callbacks;
public :
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(void);

    void addScene(ref<Graphics::Scene> scene, ref<Graphics::RenderTarget> target);
    void addWorld(ref<World> world);

    weak<const Scheduler> scheduler() const  { return m_scheduler; }
private :
    Application(void);
    Application(const Application&);
    const Application& operator=(const Application&);

    be_metaclass(NONE,Application,Object)
    be_properties
    be_end
};

}

/*****************************************************************************/
#endif
