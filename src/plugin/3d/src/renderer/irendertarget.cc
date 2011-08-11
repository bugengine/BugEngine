/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/renderer/irendertarget.hh>
#include    <3d/renderer/irenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

#include    <system/scheduler/task/task.hh>
#include    <system/scheduler/range/onestep.hh>

namespace BugEngine { namespace Graphics
{

IRenderTarget::IRenderTarget(weak<const RenderTarget> rendertarget, weak<IRenderer> renderer)
:   IGPUResource(rendertarget, renderer)
{
}

IRenderTarget::~IRenderTarget()
{
}

weak<ITask> IRenderTarget::syncTask() const
{
    return m_renderer->syncTask();
}

void IRenderTarget::drawBatches(const Batch* /*batches*/, size_t /*count*/) const
{
    begin(IRenderTarget::Clear);
    //m_renderTarget->drawBatches(m_batches);
    end(IRenderTarget::Present);
}

}}
