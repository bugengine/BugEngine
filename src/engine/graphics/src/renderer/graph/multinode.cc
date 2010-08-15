/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/graph/multinode.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

MultiNode::MultiNode()
:   INode()
,   m_renderTask(ref<TaskGroup>::create("renderMultiScene", color32(255, 0, 0)))
,   m_syncTask(ref<TaskGroup>::create("syncMultiScene", color32(255, 0, 0)))
,   m_dispatchTask(ref<TaskGroup>::create("dispatchMultiScene", color32(255, 0, 0)))
,   m_cleanTask(ref< Task< MethodCaller<MultiNode, &MultiNode::clean> > >::create("cleanNodes", color32(255,255,0), MethodCaller<MultiNode, &MultiNode::clean>(this)))
,   m_endSyncConnection(m_syncTask, m_cleanTask)
,   m_mainNodes(0)
{
    setup(m_renderTask, m_syncTask, m_dispatchTask);
}

MultiNode::~MultiNode()
{
    disconnect();
}

void MultiNode::clean()
{
    minitl::list<NodeInfo>::iterator prev = m_nodes.end();
    for(minitl::list<NodeInfo>::iterator it = m_nodes.begin(); it != m_nodes.end(); )
    {
        if(it->node->closed())
        {
            if(it->type == MainWindow)
                m_mainNodes--;
            if(prev != m_nodes.end())
            {
                minitl::list<NodeInfo>::iterator next = it;
                next++;
                if(next == m_nodes.end())
                {
                    prev->chainDispatch = ITask::CallbackConnection();
                }
                else
                {
                    prev->chainDispatch = ITask::CallbackConnection(prev->node->dispatchTask(), next->node->dispatchTask()->startCallback());
                }
            }
            it = m_nodes.erase(it);
        }
        else
        {
            prev = it;
            ++it;
        }
    }
    if(m_mainNodes == 0)
    {
        m_nodes.clear();
    }
}

void MultiNode::addNode(scoped<INode> node, NodeType type)
{
    m_nodes.push_back(NodeInfo(node, this, type));
    if(type == MainWindow)
        m_mainNodes++;
    std::list<NodeInfo>::reverse_iterator it = m_nodes.rbegin();
    be_assert(it != m_nodes.rend(), "Added node but list is still empty");
    std::list<NodeInfo>::reverse_iterator it2 = it++;
    if(it != m_nodes.rend())
    {
        it->chainDispatch = ITask::CallbackConnection(it->node->dispatchTask(), it2->node->dispatchTask()->startCallback());
    }
}

bool MultiNode::closed() const
{
    return m_mainNodes == 0;
}

weak<ITask> MultiNode::updateTask()
{
    return m_renderTask;
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


MultiNode::NodeInfo::NodeInfo(scoped<INode> n, weak<MultiNode> owner, NodeType type)
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
