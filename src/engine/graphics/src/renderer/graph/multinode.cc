/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/graph/multinode.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

MultiNode::MultiNode()
:   m_globalTask(ref<TaskGroup>::create("updateMultiScene", color32(255, 0, 0)))
,   m_renderTask(ref<TaskGroup>::create("renderMultiScene", color32(255, 0, 0)))
,   m_dispatchTask(ref<TaskGroup>::create("dispatchMultiScene", color32(255, 0, 0)))
,   m_updateTask(ref< Task< MethodCaller<MultiNode, &MultiNode::update> > >::create("updateNodes", color32(255,255,0), MethodCaller<MultiNode, &MultiNode::update>(this)))
,   m_startUpdateConnection(m_globalTask, m_updateTask)
,   m_endUpdateConnection(m_globalTask, m_dispatchTask)
,   m_startRender(m_updateTask, m_renderTask->startCallback())
,   m_startDispatch(m_renderTask, m_dispatchTask->startCallback())
,   m_mainNodes(0)
{
}

MultiNode::~MultiNode()
{
}

void MultiNode::update()
{
    minitl::list<NodeInfo>::iterator prev;
    for(minitl::list<NodeInfo>::iterator it = m_nodes.begin(); it != m_nodes.end(); )
    {
        if(it->node->closed())
        {
            if(it->type == MainWindow)
                m_mainNodes--;
            it->disconnect();
            ++it;
        }
        else
        {
            prev = it;
            ++it;
        }
    }
}

void MultiNode::addNode(scoped<INode> node, NodeType type)
{
    m_nodes.push_back(NodeInfo(node, m_updateTask, m_renderTask, m_dispatchTask, type));
    if(type == MainWindow)
        m_mainNodes++;
}

weak<ITask> MultiNode::renderTask()
{
    return m_globalTask;
}

weak<ITask> MultiNode::dispatchTask()
{
    return m_dispatchTask;
}

bool MultiNode::closed() const
{
    return m_mainNodes == 0;
}

void MultiNode::disconnect()
{
}

MultiNode::NodeInfo::NodeInfo(scoped<INode> n, ref<ITask> update, ref<TaskGroup> render, ref<TaskGroup> dispatch, NodeType type)
:   node(n)
,   renderStartConnection(render, node->renderTask())
,   renderEndConnection(render, node->renderTask())
,   dispatchStartConnection(dispatch, node->dispatchTask())
,   dispatchEndConnection(dispatch, node->dispatchTask())
,   chainFlush()
,   type(type)
{
}

MultiNode::NodeInfo::NodeInfo(const NodeInfo& other)
{
    // reverse order to ensure proper deinitialization
    type = other.type;
    chainFlush = other.chainFlush;
    renderStartConnection = other.renderStartConnection;
    dispatchEndConnection = other.dispatchEndConnection;
    dispatchStartConnection = other.dispatchStartConnection;
    renderEndConnection = other.renderEndConnection;
    node = other.node;
}

void MultiNode::NodeInfo::disconnect()
{
    node->disconnect();
    type = ToolWindow;
    chainFlush = ITask::CallbackConnection();
    renderStartConnection = TaskGroup::TaskStartConnection();
    dispatchEndConnection = TaskGroup::TaskEndConnection();
    dispatchStartConnection = TaskGroup::TaskStartConnection();
    renderEndConnection = TaskGroup::TaskEndConnection();
}

}}
