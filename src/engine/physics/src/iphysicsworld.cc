/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <physics/stdafx.h>
#include    <physics/iphysicsworld.hh>

#include    <system/scheduler/function.hh>

namespace BugEngine { namespace Physics
{

be_abstractmetaclass_impl("Physics", IPhysicsWorld);

IPhysicsWorld::IPhysicsWorld(weak<BaseTask::Callback> endJob)
:   m_start(ref< BaseTask::Callback >::create())
,   m_end(endJob)
{
    m_tasks.push_back(ref< Task< FunctionBody <IPhysicsWorld, &IPhysicsWorld::step> > >::create("physics", color32(12, 255, 12), FunctionBody <IPhysicsWorld, &IPhysicsWorld::step>(this)));
    m_start->connectTo(m_tasks[0]);
    m_end->connectFrom(m_tasks[0]);
}

IPhysicsWorld::~IPhysicsWorld()
{
}

}}

