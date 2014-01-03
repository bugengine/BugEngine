/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/renderer/irendertarget.hh>
#include    <3d/renderer/irenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

#include    <scheduler/task/task.hh>
#include    <scheduler/range/onestep.hh>

namespace BugEngine
{

IRenderTarget::IRenderTarget(weak<const RenderTargetDescription> rendertarget, weak<const IRenderer> renderer)
:   IGPUResource(rendertarget, renderer)
{
}

IRenderTarget::~IRenderTarget()
{
}

weak<Task::ITask> IRenderTarget::syncTask() const
{
    return m_renderer->syncTask();
}

void IRenderTarget::drawBatches(const Batch* /*batches*/, size_t /*count*/) const
{
    begin(IRenderTarget::Clear);
    //m_renderTarget->drawBatches(m_batches);
    end(IRenderTarget::Present);
}

}
