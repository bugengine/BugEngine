/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/world.hh>
#include    <graphics/scene/scene3d.hh>
#include    <rtti/namespace.hh>
#include    <input/action.hh>
#include    <system/scheduler/function.hh>

namespace BugEngine { namespace Graphics
{

be_abstractmetaclass_impl("Graphics",World);

World::World(weak<BaseTask::Callback> endJob)
:   m_renderer(scoped<Renderer>::create("renderOpenGL"))
,   m_start( ref< BaseTask::Callback >::create())
,   m_end(endJob)
{
    m_tasks.push_back(ref< Task< FunctionBody <World, &World::step> > >::create("window", color32(255, 12, 12), FunctionBody <World, &World::step>(this)));
    m_start->connectTo(m_tasks[0]);
    m_end->connectFrom(m_tasks[0]);
}

World::~World()
{
}

void World::step()
{
    if(m_renderer->step())
    {
        m_start->disconnectTo(m_tasks[0]);
    }
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
