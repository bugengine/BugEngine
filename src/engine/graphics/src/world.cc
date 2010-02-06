/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/world.hh>
#include    <graphics/scene/scene3d.hh>
#include    <rtti/namespace.hh>
#include    <input/action.hh>
#include    <system/scheduler/function.hh>
#include    <system/scheduler/range/sequence.hh>

namespace BugEngine { namespace Graphics
{

be_abstractmetaclass_impl("Graphics",World);

struct World::Render
{
private:
    weak<World> const       m_world;
private:
    Render& operator=(const Render& other);
public:
    typedef range_sequence<World::SceneList::const_iterator>   Range;
    Render(weak<World> world)
        :   m_world(world)
    {
    }
    ~Render()
    {
    }

    Range prepare() { return Range(m_world->m_scenes.begin(), m_world->m_scenes.end()); }
    void operator()(Range& r)
    {
        for(World::SceneList::const_iterator it = r.begin(); it != r.end(); ++it)
        {
        }
    }
    void operator()(Range& /*myRange*/, Render& /*with*/, Range& /*withRange*/)
    {
    }
};

struct World::Sort
{
private:
    weak<World> const       m_world;
private:
    Sort& operator=(const Sort& other);
public:
    typedef range_sequence<World::SceneList::const_iterator>   Range;
    Sort(weak<World> world)
        :   m_world(world)
    {
    }
    ~Sort()
    {
    }

    Range prepare() { return Range(m_world->m_scenes.begin(), m_world->m_scenes.end()); }
    void operator()(Range& r)
    {
        for(World::SceneList::const_iterator it = r.begin(); it != r.end(); ++it)
        {
        }
    }
    void operator()(Range& /*myRange*/, Sort& /*with*/, Range& /*withRange*/)
    {
    }
};

World::World(weak<BaseTask::Callback> endJob)
:   m_renderer("renderDx9")
,   m_start( ref< BaseTask::Callback >::create())
,   m_end(endJob)
{
    m_tasks.push_back(ref< Task< FunctionBody <World, &World::step> > >::create("window", color32(255, 12, 12), FunctionBody <World, &World::step>(this)));
    m_tasks.push_back(ref< Task< Render > >::create("render", color32(255, 12, 12), Render(this)));
    m_tasks.push_back(ref< Task< Sort > >::create("sort", color32(255, 12, 12), Sort(this)));
    m_tasks.push_back(ref< Task< FunctionBody <World, &World::dispatch> > >::create("dispatch", color32(255, 12, 12), FunctionBody <World, &World::dispatch>(this)));
    m_edges.push_back(ref< BaseTask::Callback >::create());
    m_edges.push_back(ref< BaseTask::Callback >::create());
    m_edges.push_back(ref< BaseTask::Callback >::create());
    m_start->connectTo(m_tasks[0]);
    m_end->connectFrom(m_tasks[3]);
    m_edges[0]->connectFrom(m_tasks[0]);
    m_edges[0]->connectTo(m_tasks[1]);
    m_edges[1]->connectFrom(m_tasks[1]);
    m_edges[1]->connectFrom(m_tasks[3], BaseTask::Callback::InitialStateCompleted);
    m_edges[1]->connectTo(m_tasks[2]);
    m_edges[2]->connectFrom(m_tasks[2]);
    m_edges[2]->connectTo(m_tasks[3]);
}

World::~World()
{
}

void World::step()
{
    if(m_renderer->step())
    {
        m_edges[0]->disconnectTo(m_tasks[1]);
    }
}

void World::dispatch()
{
    m_renderer->dispatch();
    m_renderer->flush();
}

void World::createWindow(WindowFlags f, ref<Scene> scene)
{
    ref<RenderTarget> w = m_renderer->createRenderWindow(f, scene);
    m_scenes.push_back(w);
}

}}
