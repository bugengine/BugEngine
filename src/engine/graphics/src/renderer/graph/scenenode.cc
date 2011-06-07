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
,   m_renderTask(ref< Task< MethodCaller<SceneNode, &SceneNode::render> > >::create(taskArena(), "renderScene", color32(255,0,0), MethodCaller<SceneNode, &SceneNode::render>(this), Scheduler::High))
,   m_dispatchTask(ref< Task< MethodCaller<SceneNode, &SceneNode::dispatch> > >::create(taskArena(), "dispatchScene", color32(255,0,0), MethodCaller<SceneNode, &SceneNode::dispatch>(this), Scheduler::High, renderTarget->syncTask()->affinity))
,   m_scene(scene)
,   m_renderTarget(renderTarget)
,   m_startRender(m_scene->updateTask(), m_renderTask->startCallback())
,   m_startDispatch(m_renderTask, m_dispatchTask->startCallback())
,   m_startFlush(m_dispatchTask, m_renderTarget->syncTask()->startCallback())
,   m_waitOnRender(m_renderTask, m_scene->updateTask()->startCallback(), ITask::ICallback::Completed)
,   m_waitOnFlush(m_renderTarget->syncTask(), m_renderTask->startCallback(), ITask::ICallback::Completed)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::render()
{
}

void SceneNode::dispatch()
{
    m_renderTarget->drawBatches(0, 0);
}

weak<ITask> SceneNode::updateTask()
{
    return m_scene->updateTask();
}

weak<ITask> SceneNode::dispatchTask()
{
    return m_dispatchTask;
}

}}
