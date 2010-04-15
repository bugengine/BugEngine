/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/renderer/renderbackend.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

struct RenderJob
{
    RenderJob();
};

RenderTarget::RenderTarget(weak<RenderBackend> renderer)
:   m_renderer(renderer)
{
}


ref<ITask> RenderTarget::createSceneRenderTask(weak<Scene> scene)
{
    ref<ITask> result = ref< Task< MethodCaller<RenderTarget, &RenderTarget::render> > >::create("copyWorld", color32(255,0,0),  MethodCaller<RenderTarget, &RenderTarget::render>(this));
    result->addCallback(m_renderer->flushTask()->startCallback());
    m_renderer->flushTask()->addCallback(result->startCallback(), ITask::ICallback::CallbackStatus_Completed);
    return result;
}

void RenderTarget::render()
{
}

}}
