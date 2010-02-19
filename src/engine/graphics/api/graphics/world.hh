/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_WORLD_HH_
#define BE_GRAPHICS_WORLD_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/scene/scene.hh>
#include    <system/scheduler/task.hh>
#include    <system/plugin.hh>

namespace BugEngine
{
class Scheduler;
}

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) World : public Object
{
private:
    struct Render;
    friend struct Render;
    struct Sort;
    friend struct Sort;
private:
    typedef std::pair< ref<RenderTarget>, ref<Scene> >  View;
    typedef std::vector< View>                          ViewList;
    Plugin<RenderBackend>                               m_renderer;
    ViewList                                            m_views;
public:
    void step();
    void dispatch();
public:
    World();
    ~World();

    ref<Graphics::RenderTarget> createWindow(WindowFlags f);
    void addView(ref<RenderTarget> target, ref<Scene> scene);



    be_metaclass(GRAPHICS,World, Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
