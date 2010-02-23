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
    scoped<Scheduler>       m_scheduler;
    ref<BaseTask::Callback> m_frameFinished;
    scoped< BaseTask >      m_updateInputTask;
    scoped< BaseTask >      m_updateMemoryTask;
    scoped< BaseTask >      m_updateSchedulerTask;
public :
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(void);

    ref<Graphics::RenderTarget> createWindow(Graphics::WindowFlags f);
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
