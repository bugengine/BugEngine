/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/renderer/renderbackend.hh>
#include    <system/scheduler/task/task.hh>
#include    <system/scheduler/range/onestep.hh>

namespace BugEngine { namespace Graphics
{

IRenderTarget::IRenderTarget(weak<RenderBackend> renderer)
:   m_renderer(renderer)
,   m_flushTask(ref<TaskGroup>::create("targetFlush", color32(255,0,0)))
,   m_startFlushConnection(m_flushTask, m_renderer->flushTask())
,   m_endFlushConnection(m_flushTask, m_renderer->flushTask())
{
}

IRenderTarget::~IRenderTarget()
{
}

weak<ITask> IRenderTarget::flushTask() const
{
    return m_renderer->flushTask();
}

}}
