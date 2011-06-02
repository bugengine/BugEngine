/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graph/scenenode.hh>
#include    <graphics/renderer/irendertarget.hh>
#include    <graphics/scene/iscene.script.hh>

#include    <system/scheduler/task/method.hh>


namespace BugEngine { namespace Graphics
{

SceneNode::SceneNode(weak<IScene> scene, weak<IRenderTarget> renderTarget)
:   INode()
,   m_renderTask(ref< Task< MethodCaller<SceneNode, &SceneNode::render> > >::create(taskArena(), "renderScene", color32(255,0,0), MethodCaller<SceneNode, &SceneNode::render>(this)))
,   m_scene(scene)
,   m_renderTarget(renderTarget)
,   m_startRenderConnection(m_scene->updateTask(), m_renderTask->startCallback())
,   m_startUpdateConnection(m_renderTask, m_scene->updateTask()->startCallback(), ITask::ICallback::Completed)
,   m_startFlush(m_renderTask, m_renderTarget->syncTask()->startCallback())
,   m_startRender(m_renderTarget->syncTask(), m_renderTask->startCallback(), ITask::ICallback::Completed)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::render()
{
    //m_renderTarget->begin(IRenderTarget::Clear);
    //m_renderTarget->drawBatches(m_batches);
    //m_renderTarget->end(IRenderTarget::Present);
}

weak<ITask> SceneNode::updateTask()
{
    return m_scene->updateTask();
}

weak<ITask> SceneNode::renderTask()
{
    return m_renderTask;
}

}}
