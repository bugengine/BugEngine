/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/graph/multinode.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

MultiNode::MultiNode()
:   m_task(ref<TaskGroup>::create("renderMultiScene", color32(255, 0, 0)))
,   m_updateTask(ref< Task< MethodCaller<MultiNode, &MultiNode::update> > >::create("updateNodes", color32(255,255,0), MethodCaller<MultiNode, &MultiNode::update>(this)))
,   m_startConnection(m_task, m_updateTask)
,   m_endConnection(m_task, m_updateTask)
{
}

MultiNode::~MultiNode()
{
}

void MultiNode::update()
{
    for(minitl::vector<NodeInfo>::iterator it = m_mainNodes.begin(); it != m_mainNodes.end(); )
    {
        if(it->node->closed())
        {
            it = m_mainNodes.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for(minitl::vector<NodeInfo>::iterator it = m_secondaryNodes.begin(); it != m_secondaryNodes.end(); )
    {
        if(it->node->closed())
        {
            it = m_secondaryNodes.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MultiNode::addMainNode(ref<INode> node)
{
    m_mainNodes.push_back(NodeInfo(node, m_updateTask, m_task));
}

void MultiNode::addSecondaryNode(ref<INode> node)
{
    m_secondaryNodes.push_back(NodeInfo(node, m_updateTask, m_task));
}

weak<ITask> MultiNode::renderTask()
{
    return m_task;
}

bool MultiNode::closed() const
{
    if(m_mainNodes.empty())
        return true;
    else
        return false;
}

MultiNode::NodeInfo::NodeInfo(ref<INode> node, ref<ITask> updateTask, ref<TaskGroup> owner)
:   node(node)
,   groupConnection(owner, node->renderTask())
,   startConnection(updateTask, node->renderTask()->startCallback())
{
}

MultiNode::NodeInfo::NodeInfo(const NodeInfo& other)
{
    startConnection = other.startConnection;
    groupConnection = other.groupConnection;
    node = other.node;
}

MultiNode::NodeInfo& MultiNode::NodeInfo::operator=(const NodeInfo& other)
{
    startConnection = other.startConnection;
    groupConnection = other.groupConnection;
    node = other.node;
    return *this;
}


}}
