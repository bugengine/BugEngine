/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_WORLD_HH_
#define BE_GRAPHICS_WORLD_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderer.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/scene/scene.hh>
#include    <system/scheduler/task.hh>

namespace BugEngine
{
class Scheduler;
}

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) World : public Object
{
private:
    class UpdateWindowManagement;
private:
    typedef std::vector< ref<RenderTarget> > SceneList;
    scoped<Renderer>             m_renderer;
    SceneList                    m_scenes;
    ref< BaseTask >              m_updateWindowTask;
    ref< BaseTask >              m_flushTask;
private:
    int step();
    void flush();
public:
    World();
    ~World();

    void createWindow(WindowFlags f, ref<Scene> scene);

    be_metaclass(GRAPHICS,World, Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
