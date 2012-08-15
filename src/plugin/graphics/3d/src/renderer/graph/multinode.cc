/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <graph/multinode.hh>
#include    <scheduler/task/method.hh>

namespace BugEngine
{

MultiNode::MultiNode(const minitl::vector< minitl::weak<INode> >& nodes)
:   INode()
,   m_updateTask(ref<Task::TaskGroup>::create(Arena::task(), "updateMultiScene", Colors::Red::Red))
,   m_dispatchTask(ref< Task::Task< Task::MethodCaller<MultiNode, &MultiNode::dispatch> > >::create(Arena::task(), "dispatchMulti", Colors::Red::Red, Task::MethodCaller<MultiNode, &MultiNode::dispatch>(this), Scheduler::High))
,   m_startDispatchConnection(m_updateTask, m_dispatchTask->startCallback())
,   m_waitOnDispatchConnection(m_dispatchTask, m_dispatchTask->startCallback(), Task::ITask::ICallback::Completed)
,   m_nodes(Arena::task())
{
    for(minitl::vector< weak<INode> >::const_iterator node = nodes.begin(); node != nodes.end(); ++node)
    {
        m_nodes.push_back(NodeInfo(*node, this, m_nodes.empty() ? weak<INode>() : m_nodes.back().node));
    }
}

MultiNode::~MultiNode()
{
}

weak<Task::ITask> MultiNode::updateTask()
{
    return m_updateTask;
}

weak<Task::ITask> MultiNode::dispatchTask()
{
    return m_dispatchTask;
}

void MultiNode::dispatch()
{
}

MultiNode::NodeInfo::NodeInfo(weak<INode> n, weak<MultiNode> owner, weak<INode> previous)
:   node(n)
,   chainUpdate(node->updateTask(), owner->m_updateTask->startCallback())
,   chainDispatch(previous ? Task::ITask::CallbackConnection(previous->dispatchTask(), n->dispatchTask()->startCallback()) : Task::ITask::CallbackConnection())
,   chainGlobalDispatch(node->dispatchTask(), owner->m_dispatchTask->startCallback())
{
}

}
