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
    typedef std::vector< ref<RenderTarget> > SceneList;
    Plugin<RenderBackend>                       m_renderer;
    SceneList                                   m_scenes;
    ref< BaseTask::Callback >                   m_start;
    weak< BaseTask::Callback >                  m_end;
    std::vector< ref< BaseTask > >              m_tasks;
    std::vector< ref< BaseTask::Callback > >    m_edges;
private:
    void step();
    void dispatch();
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
