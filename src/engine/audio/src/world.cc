/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <audio/stdafx.h>
#include    <audio/world.hh>

#include    <system/scheduler/function.hh>

namespace BugEngine { namespace Audio
{

be_abstractmetaclass_impl("Audio",World);

World::World(weak<BaseTask::Callback> endJob)
:   m_start(ref< BaseTask::Callback >::create())
,   m_end(endJob)
{
    m_tasks.push_back(ref< Task< FunctionBody <World, &World::step> > >::create("audio", color32(12, 255, 12), FunctionBody <World, &World::step>(this)));
    m_start->connectTo(m_tasks[0]);
    m_end->connectFrom(m_tasks[0]);
}

World::~World()
{
}

}}

