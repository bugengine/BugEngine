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
    typedef std::vector< ref<RenderTarget> > SceneList;
    scoped<Renderer>                            m_renderer;
    SceneList                                   m_scenes;
    ref< BaseTask::Callback >                   m_start;
    weak< BaseTask::Callback >                  m_end;
    std::vector< ref< BaseTask > >              m_tasks;
    std::vector< ref< BaseTask::Callback > >    m_edges;
private:
    void step();
    void flush();
public:
    World(weak<BaseTask::Callback> endJob);
    ~World();

    void createWindow(WindowFlags f, ref<Scene> scene);

    weak< BaseTask::Callback > getStart() { return m_start; }
    weak< BaseTask::Callback > getEnd()   { return m_end; }

    be_metaclass(GRAPHICS,World, Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
