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
    ref<ITask::ChainCallback> callback = ref<ITask::ChainCallback>::create();
    result->addCallback(callback);
    callback->makeStart(m_renderer->flushTask());
    ref<ITask::ChainCallback> waitPreviousFrame = ref<ITask::ChainCallback>::create();
    m_renderer->flushTask()->addCallback(waitPreviousFrame);
    callback->makeStart(result);
    return result;
}

void RenderTarget::render()
{
}

}}
