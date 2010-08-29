/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/graph/multinode.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

MultiNode::MultiNode()
:   INode()
,   m_globalTask(ref<TaskGroup>::create<Arena::General>("updateMultiScene", color32(255, 0, 0)))
,   m_renderTask(ref<TaskGroup>::create<Arena::General>("renderMultiScene", color32(255, 0, 0)))
,   m_syncTask(ref<TaskGroup>::create<Arena::General>("syncMultiScene", color32(255, 0, 0)))
,   m_dispatchTask(ref<TaskGroup>::create<Arena::General>("dispatchMultiScene", color32(255, 0, 0)))
,   m_cleanTask(ref< Task< MethodCaller<MultiNode, &MultiNode::clean> > >::create<Arena::General>("cleanNodes", color32(255,255,0), MethodCaller<MultiNode, &MultiNode::clean>(this)))
,   m_endSyncConnection(m_syncTask, m_cleanTask)
,   m_startGlobalConnection(m_globalTask, m_renderTask)
,   m_endGlobalConnection(m_globalTask, m_syncTask)
,   m_jobGraph(m_renderTask, m_syncTask, m_dispatchTask)
,   m_mainNodes(0)
{
}

MultiNode::~MultiNode()
{
}

void MultiNode::clean()
{
    for(minitl::vector<NodeInfo, Arena::General>::iterator it = m_nodes.begin(); it != m_nodes.end(); )
    {
        if(it->node->closed())
        {
            if(it->type == MainWindow)
                m_mainNodes--;
            if(it != m_nodes.begin())
            {
                if(it+1 != m_nodes.end())
                {
                    (it-1)->chainDispatch = ITask::CallbackConnection();
                }
                else
                {
                    (it-1)->chainDispatch = ITask::CallbackConnection((it-1)->node->dispatchTask(), (it+1)->node->dispatchTask()->startCallback());
                }
            }
            it = m_nodes.erase(it);
        }
        else
        {
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
    minitl::vector<NodeInfo, Arena::General>::reverse_iterator it = m_nodes.rbegin();
    be_assert(it != m_nodes.rend(), "Added node but list is still empty");
    minitl::vector<NodeInfo, Arena::General>::reverse_iterator it2 = it++;
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
