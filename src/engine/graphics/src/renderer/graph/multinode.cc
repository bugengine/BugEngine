/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graph/multinode.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

MultiNode::MultiNode(const minitl::vector< minitl::weak<INode> >& nodes)
:   INode()
,   m_globalTask(ref<TaskGroup>::create(taskArena(), "updateMultiScene", color32(255, 0, 0)))
,   m_renderTask(ref<TaskGroup>::create(taskArena(), "renderMultiScene", color32(255, 0, 0)))
,   m_syncTask(ref<TaskGroup>::create(taskArena(), "syncMultiScene", color32(255, 0, 0)))
,   m_dispatchTask(ref<TaskGroup>::create(taskArena(), "dispatchMultiScene", color32(255, 0, 0)))
,   m_cleanTask(ref< Task< MethodCaller<MultiNode, &MultiNode::clean> > >::create(taskArena(), "cleanNodes", color32(255,255,0), MethodCaller<MultiNode, &MultiNode::clean>(this)))
,   m_endSyncConnection(m_syncTask, m_cleanTask)
,   m_startGlobalConnection(m_globalTask, m_renderTask)
,   m_endGlobalConnection(m_globalTask, m_syncTask)
,   m_jobGraph(m_renderTask, m_syncTask, m_dispatchTask)
,   m_nodes(taskArena())
,   m_mainNodes(0)
{
    for(minitl::vector< weak<INode> >::const_iterator node = nodes.begin(); node != nodes.end(); ++node)
    {
        m_nodes.push_back(NodeInfo(*node, this, MainWindow));
        m_mainNodes++;
        minitl::vector<NodeInfo>::reverse_iterator it = m_nodes.rbegin();
        be_assert(it != m_nodes.rend(), "Added node but list is still empty");
        minitl::vector<NodeInfo>::reverse_iterator it2 = it++;
        if (it != m_nodes.rend())
        {
            it->chainDispatch = ITask::CallbackConnection(it->node->dispatchTask(), it2->node->dispatchTask()->startCallback());
        }
    }
}

MultiNode::~MultiNode()
{
}

void MultiNode::clean()
{
}

weak<ITask> MultiNode::updateTask()
{
    return m_globalTask;
}

weak<ITask> MultiNode::renderTask()
{
    return m_renderTask;
}

weak<ITask> MultiNode::syncTask()
{
    return m_syncTask;
}

weak<ITask> MultiNode::dispatchTask()
{
    return m_dispatchTask;
}


MultiNode::NodeInfo::NodeInfo(weak<INode> n, weak<MultiNode> owner, NodeType type)
:   node(n)
,   renderStartConnection(owner->m_renderTask, node->renderTask())
,   renderEndConnection(owner->m_renderTask, node->renderTask())
,   syncStartConnection(owner->m_syncTask, node->syncTask())
,   syncEndConnection(node->syncTask(), owner->m_cleanTask->startCallback())
,   dispatchStartConnection(owner->m_dispatchTask, node->dispatchTask())
,   dispatchEndConnection(owner->m_dispatchTask, node->dispatchTask())
,   chainDispatch()
,   type(type)
{
}

}}
