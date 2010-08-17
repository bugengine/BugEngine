/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/renderbackend.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

RenderBackend::RenderBackend()
:   m_syncTask(ref< Task< MethodCaller<RenderBackend, &RenderBackend::flush> > >::create("flush", color32(255,0,0),  MethodCaller<RenderBackend, &RenderBackend::flush>(this), Scheduler::High))
{
}

RenderBackend::~RenderBackend()
{
}

weak<ITask> RenderBackend::syncTask()
{
    return m_syncTask;
}

}}
