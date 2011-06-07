/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/irendertarget.hh>
#include    <graphics/renderer/irenderer.hh>
#include    <graphics/objects/rendertarget.script.hh>

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
    begin(IRenderTarget::DontClear);
    //m_renderTarget->drawBatches(m_batches);
    end(IRenderTarget::Present);
}

}}
