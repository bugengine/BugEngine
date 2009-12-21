/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/world.hh>

namespace BugEngine
{

be_abstractmetaclass_impl("",World);

World::World(float3 worldExtents)
:   m_physicsSystem("physicsBullet")
,   m_graphicsWorld(ref<Graphics::World>::create())
,   m_physicsWorld(m_physicsSystem->createWorld(worldExtents))
,   m_soundWorld("audioOpenAL")
{
}

World::~World()
{
}

void World::createView(Graphics::WindowFlags f, ref<Graphics::Scene> scene)
{
    m_graphicsWorld->createWindow(f, scene);
}

}
