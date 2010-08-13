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
{
}

IRenderTarget::~IRenderTarget()
{
}

weak<ITask> IRenderTarget::syncTask() const
{
    return m_renderer->syncTask();
}

}}
