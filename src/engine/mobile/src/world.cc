/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/world.hh>

#include    <system/scheduler/function.hh>


namespace BugEngine
{

be_abstractmetaclass_impl("",World);

World::World(float3 worldExtents)
:   m_physicsSystem("physicsBullet")
,   m_audioSystem("audioOpenAL")
{
}

World::~World()
{
}

}
