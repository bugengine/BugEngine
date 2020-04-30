/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/irenderer.hh>
#include <bugengine/plugin.graphics.3d/renderer/irendertarget.hh>
#include <bugengine/plugin.graphics.3d/rendertarget/rendertarget.script.hh>

#include <bugengine/scheduler/range/onestep.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine {

IRenderTarget::IRenderTarget(weak< const RenderTargetDescription > rendertarget,
                             weak< const IRenderer >               renderer)
    : IGPUResource(rendertarget, renderer)
{
}

IRenderTarget::~IRenderTarget()
{
}

weak< Task::ITask > IRenderTarget::syncTask() const
{
    return m_renderer->syncTask();
}

void IRenderTarget::drawBatches(const Batch* /*batches*/, size_t /*count*/) const
{
    begin(IRenderTarget::Clear);
    // m_renderTarget->drawBatches(m_batches);
    end(IRenderTarget::Present);
}

}  // namespace BugEngine
