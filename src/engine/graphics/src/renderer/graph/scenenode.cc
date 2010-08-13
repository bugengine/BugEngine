/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/graph/scenenode.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/scene/iscene.hh>

#include    <system/scheduler/task/method.hh>


namespace BugEngine { namespace Graphics
{

SceneNode::RenderConfig::RenderConfig(weak<SceneNode> node)
:   m_renderTask(ref<TaskGroup>::create("renderGroup", color32(255, 0, 0)))
,   m_renderStartTask(m_renderTask, node->m_scene->updateTask())
,   m_renderEndTask(m_renderTask, node->m_scene->updateTask())
{
}

void SceneNode::RenderConfig::disconnect()
{
    m_renderStartTask = TaskGroup::TaskStartConnection();
    m_renderEndTask = TaskGroup::TaskEndConnection();
}

SceneNode::DispatchConfig::DispatchConfig(weak<SceneNode> node)
:   m_syncTask(node->m_renderTarget->syncTask())
,   m_realDispatch(ref< Task< MethodCaller<SceneNode, &SceneNode::dispatch> > >::create("dispatch", color32(255,255,0), MethodCaller<SceneNode, &SceneNode::dispatch>(node)))
,   m_startRealDispatch(m_syncTask, m_realDispatch->startCallback())
,   m_waitForSync(m_realDispatch, m_syncTask->startCallback(), ITask::ICallback::Completed)
{
}

void SceneNode::DispatchConfig::disconnect()
{
    m_startRealDispatch = ITask::CallbackConnection();
    m_waitForSync = ITask::CallbackConnection();
}

SceneNode::SceneNode(ref<IScene> scene, ref<IRenderTarget> renderTarget)
:   m_scene(scene)
,   m_renderTarget(renderTarget)
,   m_renderConfig(this)
,   m_dispatchConfig(this)
,   m_startDispatch(m_renderConfig.m_renderTask, m_dispatchConfig.m_syncTask->startCallback())
,   m_waitSync(m_dispatchConfig.m_syncTask, m_renderConfig.m_renderTask->startCallback(), ITask::ICallback::Completed)
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
    return m_dispatchConfig.m_syncTask;
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

void SceneNode::disconnect()
{
    m_startDispatch = ITask::CallbackConnection();
    m_waitSync = ITask::CallbackConnection();
    m_renderConfig.disconnect();
    m_dispatchConfig.disconnect();
}

}}
