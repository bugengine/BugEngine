/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <rtti/helper.hh>

#include    <mobile/world.hh>
#include    <graphics/scene/scene.hh>
#include    <main/frame.hh>

namespace BugEngine
{

class Application : public Object
{
private:
    class UpdateInput;
    class UpdateScheduler;
    class UpdateMemory;
private:
    scoped<Scheduler>    m_scheduler;
    ref<World>           m_world;
    scoped< BaseTask >   m_updateInputTask;
    scoped< BaseTask >   m_updateMemoryTask;
    scoped< BaseTask >   m_updateSchedulerTask;
    ref<Frame>           m_frame;
public :
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(void);

    void createWindow(Graphics::WindowFlags f, ref<Graphics::Scene> scene);

    weak<const Scheduler> scheduler() const  { return m_scheduler; }
private :
    Application(void);
    Application(const Application&);
    const Application& operator=(const Application&);

    be_metaclass(NONE,Application,Object)
    be_properties
        be_method(run);

        be_property(world)
            [be_read(m_world)]
            [be_write(m_world)];
    be_end
};

}

/*****************************************************************************/
#endif
