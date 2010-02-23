/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <physics/stdafx.h>
#include    <physics/iphysicsworld.hh>

#include    <system/scheduler/function.hh>

namespace BugEngine { namespace Physics
{

be_abstractmetaclass_impl("Physics", IPhysicsWorld);

IPhysicsWorld::IPhysicsWorld()
{
}

IPhysicsWorld::~IPhysicsWorld()
{
}

}}

