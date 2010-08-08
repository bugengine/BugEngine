/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/graph/scenenode.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/scene/iscene.hh>

#include    <system/scheduler/task/method.hh>


namespace BugEngine { namespace Graphics
{

SceneNode::RenderConfig::RenderConfig(SceneNode* /*node*/, weak<IScene> scene)
:   m_renderTask(ref<TaskGroup>::create("renderGroup", color32(255, 0, 0)))
,   m_renderStartTask(m_renderTask, scene->updateTask())
,   m_renderEndTask(m_renderTask, scene->updateTask())
{
}

SceneNode::DispatchConfig::DispatchConfig(SceneNode* node, weak<IRenderTarget> renderTarget)
:   m_dispatchTask(renderTarget->flushTask())
,   m_realDispatch(ref< Task< MethodCaller<SceneNode, &SceneNode::dispatch> > >::create("dispatch", color32(255,255,0), MethodCaller<SceneNode, &SceneNode::dispatch>(node)))
,   m_startRealDispatch(m_dispatchTask, m_realDispatch->startCallback())
,   m_waitForSync(m_realDispatch, m_dispatchTask->startCallback(), ITask::ICallback::Completed)
{
}

SceneNode::SceneNode(ref<IScene> scene, ref<IRenderTarget> renderTarget)
:   m_scene(scene)
,   m_renderTarget(renderTarget)
,   m_renderConfig(this, m_scene)
,   m_dispatchConfig(this, m_renderTarget)
,   m_startDispatch(m_renderConfig.m_renderTask, m_dispatchConfig.m_dispatchTask->startCallback())
{
}

SceneNode::~SceneNode()
{
}

weak<ITask> SceneNode::renderTask()
{
    return  m_renderConfig.m_renderTask;
}

weak<ITask> SceneNode::dispatchTask()
{
    return m_dispatchConfig.m_dispatchTask;
}

bool SceneNode::closed() const
{
    return m_renderTarget->closed();
}

void SceneNode::dispatch()
{
    m_renderTarget->begin(IRenderTarget::Clear);
    //m_renderTarget->drawBatches(m_batches);
    m_renderTarget->end(IRenderTarget::Present);
}

}}
