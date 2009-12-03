/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/world.hh>
#include    <graphics/scene/scene3d.hh>
#include    <rtti/namespace.hh>
#include    <input/action.hh>
#include    <system/scheduler/range/onestep.hh>

namespace BugEngine { namespace Graphics
{

be_metaclass_impl("Graphics",World);

class World::UpdateWindowManagement
{
    friend class Task<UpdateWindowManagement>;
private:
    typedef range_onestep   Range;
    World*                  m_world;
public:
    UpdateWindowManagement(World* world)
        :   m_world(world)
    {
    }
    ~UpdateWindowManagement()
    {
    }

    range_onestep prepare() { return range_onestep(); }
    void operator()(range_onestep& /*r*/)
    {
        m_world->step();
    }
    void operator()(range_onestep& /*myRange*/, UpdateWindowManagement& /*with*/, range_onestep& /*withRange*/)
    {
    }
};

World::World()
:   m_renderer(scoped<Renderer>::create("renderOpenGL"))
,   m_updateWindowTask(ref< Task<UpdateWindowManagement> >::create("window", color32(255, 12, 12), UpdateWindowManagement(this)))
{
}

World::~World()
{
}

int World::step()
{
    return m_renderer->step();
}

void World::flush()
{
}

void World::createWindow(WindowFlags f, ref<Scene> scene)
{
    ref<RenderTarget> w = m_renderer->createRenderWindow(f, scene);
    m_scenes.push_back(w);
}

}}
