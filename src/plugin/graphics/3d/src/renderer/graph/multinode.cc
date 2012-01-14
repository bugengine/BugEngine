/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <graph/multinode.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine
{

MultiNode::MultiNode(const minitl::vector< minitl::weak<INode> >& nodes)
:   INode()
,   m_updateTask(ref<TaskGroup>::create(taskArena(), "updateMultiScene", color32(255, 0, 0)))
,   m_dispatchTask(ref< Task< MethodCaller<MultiNode, &MultiNode::dispatch> > >::create(taskArena(), "dispatchMulti", color32(255,0,0), MethodCaller<MultiNode, &MultiNode::dispatch>(this), Scheduler::High))
,   m_startDispatchConnection(m_updateTask, m_dispatchTask->startCallback())
,   m_waitOnDispatchConnection(m_dispatchTask, m_dispatchTask->startCallback(), ITask::ICallback::Completed)
,   m_nodes(taskArena())
{
    for(minitl::vector< weak<INode> >::const_iterator node = nodes.begin(); node != nodes.end(); ++node)
    {
        m_nodes.push_back(NodeInfo(*node, this, m_nodes.empty() ? weak<INode>() : m_nodes.back().node));
    }
}

MultiNode::~MultiNode()
{
}

weak<ITask> MultiNode::updateTask()
{
    return m_updateTask;
}

weak<ITask> MultiNode::dispatchTask()
{
    return m_dispatchTask;
}

void MultiNode::dispatch()
{
}

MultiNode::NodeInfo::NodeInfo(weak<INode> n, weak<MultiNode> owner, weak<INode> previous)
:   node(n)
,   chainUpdate(node->updateTask(), owner->m_updateTask->startCallback())
,   chainDispatch(previous ? ITask::CallbackConnection(previous->dispatchTask(), n->dispatchTask()->startCallback()) : ITask::CallbackConnection())
,   chainGlobalDispatch(node->dispatchTask(), owner->m_dispatchTask->startCallback())
{
}

}
