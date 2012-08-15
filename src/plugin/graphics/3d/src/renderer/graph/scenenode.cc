/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <graph/scenenode.hh>
#include    <3d/renderer/irendertarget.hh>
#include    <world/world.script.hh>
#include    <scheduler/task/method.hh>


namespace BugEngine
{

SceneNode::SceneNode(weak<IRenderTarget> renderTarget, weak<const BugEngine::World::World> world)
:   INode()
,   m_world(world)
,   m_renderTask(ref< Task::Task< Task::MethodCaller<SceneNode, &SceneNode::render> > >::create(Arena::task(), "renderScene", Colors::Red::Red, Task::MethodCaller<SceneNode, &SceneNode::render>(this), Scheduler::High))
,   m_dispatchTask(ref< Task::Task< Task::MethodCaller<SceneNode, &SceneNode::dispatch> > >::create(Arena::task(), "dispatchScene", Colors::Red::Red, Task::MethodCaller<SceneNode, &SceneNode::dispatch>(this), Scheduler::High, renderTarget->syncTask()->affinity))
,   m_renderTarget(renderTarget)
,   m_startRender(world->updateWorldTask(), m_renderTask->startCallback())
,   m_startDispatch(m_renderTask, m_dispatchTask->startCallback())
,   m_startFlush(m_dispatchTask, m_renderTarget->syncTask()->startCallback())
,   m_waitOnRender(m_renderTask, world->updateWorldTask()->startCallback(), Task::ITask::ICallback::Completed)
,   m_waitOnFlush(m_renderTarget->syncTask(), m_renderTask->startCallback(), Task::ITask::ICallback::Completed)
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

weak<Task::ITask> SceneNode::updateTask()
{
    return m_world->updateWorldTask();
}

weak<Task::ITask> SceneNode::dispatchTask()
{
    return m_dispatchTask;
}

}
