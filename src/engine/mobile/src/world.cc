/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/world.hh>

namespace BugEngine
{

be_abstractmetaclass_impl("",World);

World::World(float3 worldExtents)
:   m_graphicsSystem(new Graphics::World())
,   m_physicsSystem(new Physics::World(worldExtents))
,   m_soundSystem(new Sound::World())
{
}

World::~World()
{
}

void World::createView(Graphics::WindowFlags f, refptr<Graphics::Scene> scene)
{
    m_graphicsSystem->createWindow(f, scene);
}

}
