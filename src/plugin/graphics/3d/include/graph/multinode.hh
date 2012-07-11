/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_MULTINODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_MULTINODE_HH_
/*****************************************************************************/
#include    <graph/inode.hh>
#include    <system/scheduler/task/group.hh>


namespace BugEngine
{

class MultiNode : public INode
{
private:
    struct NodeInfo
    {
        weak<INode>                     node;
        Task::ITask::CallbackConnection chainUpdate;
        Task::ITask::CallbackConnection chainDispatch;
        Task::ITask::CallbackConnection chainGlobalDispatch;
        NodeInfo(weak<INode> node, weak<MultiNode> owner, weak<INode> prevNode);
    };
    friend struct NodeInfo;
private:
    ref<Task::TaskGroup>            m_updateTask;
    ref<Task::ITask>                m_dispatchTask;
    Task::ITask::CallbackConnection m_startDispatchConnection;
    Task::ITask::CallbackConnection m_waitOnDispatchConnection;
    minitl::vector<NodeInfo>        m_nodes;
private:
    void dispatch();
public:
    MultiNode(const minitl::vector< minitl::weak<INode> >& nodes);
    ~MultiNode();

    virtual weak<Task::ITask> updateTask() override;
    virtual weak<Task::ITask> dispatchTask() override;
};

}


/*****************************************************************************/
#endif
