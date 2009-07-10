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
:   m_renderer(new Renderer("RenderDx9"))
,   m_updateWindowTask(new Task<UpdateWindowManagement>("window", color32(255, 12, 12), UpdateWindowManagement(this)))
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

void World::createWindow(WindowFlags f, refptr<Scene> scene)
{
    RenderTarget* w = m_renderer->createRenderWindow(f, scene.get());
    m_scenes.push_back(w);
}

}}
