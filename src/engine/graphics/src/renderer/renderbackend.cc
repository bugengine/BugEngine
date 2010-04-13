/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/renderbackend.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

RenderBackend::RenderBackend()
:   m_flushTask(ref< Task< MethodCaller<RenderBackend, &RenderBackend::flush> > >::create("flush", color32(255,0,0),  MethodCaller<RenderBackend, &RenderBackend::flush>(this)))
,   m_batchPool(8192)
,   m_batches()
{
}

RenderBackend::~RenderBackend()
{
}

weak<ITask> RenderBackend::flushTask()
{
    return m_flushTask;
}

}}
