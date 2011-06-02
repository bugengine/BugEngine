/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graph/multinode.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

MultiNode::MultiNode(const minitl::vector< minitl::weak<INode> >& nodes)
:   INode()
,   m_updateTask(ref<TaskGroup>::create(taskArena(), "updateMultiScene", color32(255, 0, 0)))
,   m_renderTask(ref<TaskGroup>::create(taskArena(), "renderMultiScene", color32(255, 0, 0)))
,   m_startRenderConnection(m_updateTask, m_renderTask->startCallback())
,   m_startUpdateConnection(m_renderTask, m_updateTask->startCallback(), ITask::ICallback::Completed)
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

weak<ITask> MultiNode::renderTask()
{
    return m_renderTask;
}

MultiNode::NodeInfo::NodeInfo(weak<INode> n, weak<MultiNode> owner, weak<INode> previous)
:   node(n)
,   chainUpdate(node->updateTask(), owner->m_updateTask->startCallback())
,   renderStartConnection(owner->m_renderTask, node->renderTask())
,   renderEndConnection(owner->m_renderTask, node->renderTask())
,   chainRender(previous ? ITask::CallbackConnection(previous->renderTask(), n->renderTask()->startCallback()) : ITask::CallbackConnection())
{
}

}}
