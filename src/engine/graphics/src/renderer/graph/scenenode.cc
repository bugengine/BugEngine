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
,   m_renderTask(ref<TaskGroup>::create(taskArena(), "renderScene", color32(255, 0, 0)))
,   m_dispatchTask(ref< Task< MethodCaller<SceneNode, &SceneNode::dispatch> > >::create(taskArena(), "dispatch", color32(255,255,0), MethodCaller<SceneNode, &SceneNode::dispatch>(this), Scheduler::High))
,   m_scene(scene)
,   m_renderTarget(renderTarget)
,   m_jobGraph(m_renderTask, m_renderTarget->syncTask(), m_dispatchTask)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::dispatch()
{
    m_renderTarget->begin(IRenderTarget::Clear);
    //m_renderTarget->drawBatches(m_batches);
    m_renderTarget->end(IRenderTarget::Present);
}

weak<ITask> SceneNode::updateTask()
{
    return m_scene->updateTask();
}

weak<ITask> SceneNode::renderTask()
{
    return m_renderTask;
}

weak<ITask> SceneNode::syncTask()
{
    return m_renderTarget->syncTask();
}

weak<ITask> SceneNode::dispatchTask()
{
    return m_dispatchTask;
}

}}
