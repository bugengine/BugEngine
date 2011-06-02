/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_MULTINODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_MULTINODE_HH_
/*****************************************************************************/
#include    <graph/inode.hh>
#include    <system/scheduler/task/group.hh>


namespace BugEngine { namespace Graphics
{

class MultiNode : public INode
{
private:
    struct NodeInfo
    {
        weak<INode>                     node;
        ITask::CallbackConnection       chainUpdate;
        TaskGroup::TaskStartConnection  renderStartConnection;
        TaskGroup::TaskEndConnection    renderEndConnection;
        ITask::CallbackConnection       chainRender;
        NodeInfo(weak<INode> node, weak<MultiNode> owner, weak<INode> prevNode);
    };
    friend struct NodeInfo;
private:
    ref<TaskGroup>                  m_updateTask;
    ref<TaskGroup>                  m_renderTask;
    ITask::CallbackConnection       m_startUpdateConnection;
    ITask::CallbackConnection       m_startRenderConnection;
    minitl::vector<NodeInfo>        m_nodes;
public:
    MultiNode(const minitl::vector< minitl::weak<INode> >& nodes);
    ~MultiNode();

    virtual weak<ITask> updateTask() override;
    virtual weak<ITask> renderTask() override;
};

}}


/*****************************************************************************/
#endif
