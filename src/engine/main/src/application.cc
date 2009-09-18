/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/application.hh>

#include    <rtti/namespace.hh>
#include    <system/scheduler/range/onestep.hh>
#include    <input/inputmap.hh>
#include    <input/manager.hh>

namespace BugEngine
{

be_abstractmetaclass_impl("",Application);

class Application::UpdateInput
{
    friend class Task<UpdateInput>;
private:
    typedef range_onestep   Range;
public:
    UpdateInput()
    {
    }
    ~UpdateInput()
    {
    }

    range_onestep prepare() { return range_onestep(); }
    void operator()(range_onestep& /*r*/)
    {
        Input::InputMap::static_metaclass()->getManager()->update();
    }
    void operator()(range_onestep& /*myRange*/, UpdateInput& /*with*/, range_onestep& /*withRange*/)
    {
    }
};

class Application::UpdateMemory
{
    friend class Task<UpdateMemory>;
private:
    typedef range_onestep   Range;
public:
    UpdateMemory()
    {
    }
    ~UpdateMemory()
    {
    }

    range_onestep prepare() { return range_onestep(); }
    void operator()(range_onestep& /*r*/)
    {
        Malloc::frameUpdate();
    }
    void operator()(range_onestep& /*myRange*/, UpdateMemory& /*with*/, range_onestep& /*withRange*/)
    {
    }
};

class Application::UpdateScheduler
{
    friend class Task<UpdateScheduler>;
private:
    typedef range_onestep   Range;
    Scheduler*              m_scheduler;
public:
    UpdateScheduler(Scheduler* sc)
        :   m_scheduler(sc)
    { 
    }
    ~UpdateScheduler()
    {
    }

    range_onestep prepare() { return range_onestep(); }
    void operator()(range_onestep& /*r*/)
    {
        m_scheduler->frameUpdate();
    }
    void operator()(range_onestep& /*myRange*/, UpdateScheduler& /*with*/, range_onestep& /*withRange*/)
    {
    }
};



Application::Application(int argc, const char *argv[])
:   Object()
,   m_scheduler(new Scheduler())
,   m_world(new World(float3(10000.0f, 10000.0f, 10000.0f)))
,   m_updateInputTask(new Task<UpdateInput>("input", color32(200,200,120), UpdateInput()))
,   m_updateMemoryTask(new Task<UpdateMemory>("memory", color32(150,180,120), UpdateMemory()))
,   m_updateSchedulerTask(new Task<UpdateScheduler>("scheduler", color32(200,180,120), UpdateScheduler(m_scheduler.get())))
{
    UNUSED(argc); UNUSED(argv);
}

Application::~Application(void)
{
}

int Application::run()
{
    //m_world->run(m_scheduler.get());
    //m_scheduler->wait();
    return 0;
}

void Application::createWindow(Graphics::WindowFlags f, refptr<Graphics::Scene> scene)
{
    m_world->createView(f, scene);
}

}
