/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/world.hh>

namespace BugEngine
{

be_abstractmetaclass_impl("",World);

World::World(float3 worldExtents)
:   m_graphicsSystem(ref<Graphics::World>::create())
,   m_physicsSystem(ref<Physics::World>::create(worldExtents))
,   m_soundSystem(ref<Sound::World>::create())
{
}

World::~World()
{
}

void World::createView(Graphics::WindowFlags f, ref<Graphics::Scene> scene)
{
    m_graphicsSystem->createWindow(f, scene);
}

}
