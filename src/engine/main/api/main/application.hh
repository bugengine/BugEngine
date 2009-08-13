/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <core/utils/singleton.hh>
#include    <rtti/helper.hh>

#include    <mobile/world.hh>
#include    <graphics/scene/scene.hh>
#include    <main/frame.hh>

namespace BugEngine
{

class Application : public Object, public Singleton<Application>
{
private:
    class UpdateInput;
    class UpdateScheduler;
    class UpdateMemory;
private:
    scopedptr<Scheduler>    m_scheduler;
    refptr<World>           m_world;
    scopedptr< BaseTask >   m_updateInputTask;
    scopedptr< BaseTask >   m_updateMemoryTask;
    scopedptr< BaseTask >   m_updateSchedulerTask;
    refptr<Frame>           m_frame;
public :
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(void);

    void createWindow(Graphics::WindowFlags f, refptr<Graphics::Scene> scene);

    const Scheduler* scheduler() const  { return m_scheduler.get(); }
private :
    Application(void);
    Application(const Application&);
    const Application& operator=(const Application&);

    be_metaclass(NOEXPORT,Application,Object)
        Application* instance() const { return Application::instance(); }
    be_properties
        be_classmethod(instance);

        be_method(run);

        be_property(world)
            [be_read(m_world)]
            [be_write(m_world)];
    be_end
};

}

/*****************************************************************************/
#endif
