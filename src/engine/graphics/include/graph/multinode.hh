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
public:
    enum NodeType
    {
        ToolWindow,
        MainWindow
    };
private:
    struct NodeInfo
    {
        ref<INode>                      node;
        TaskGroup::TaskStartConnection  renderStartConnection;
        TaskGroup::TaskEndConnection    renderEndConnection;
        TaskGroup::TaskStartConnection  syncStartConnection;
        ITask::CallbackConnection       syncEndConnection;
        TaskGroup::TaskStartConnection  dispatchStartConnection;
        TaskGroup::TaskEndConnection    dispatchEndConnection;
        ITask::CallbackConnection       chainDispatch;
        NodeType                        type;
        NodeInfo(scoped<INode> node, weak<MultiNode> owner, NodeType type);
    };
    friend struct NodeInfo;
private:
    ref<TaskGroup>                  m_globalTask;
    ref<TaskGroup>                  m_renderTask;
    ref<TaskGroup>                  m_syncTask;
    ref<TaskGroup>                  m_dispatchTask;
    ref<ITask>                      m_cleanTask;
    TaskGroup::TaskEndConnection    m_endSyncConnection;
    TaskGroup::TaskStartConnection  m_startGlobalConnection;
    TaskGroup::TaskEndConnection    m_endGlobalConnection;
    AsyncDispatchJobGraph           m_jobGraph;
    minitl::vector<NodeInfo>        m_nodes;
    u32                             m_mainNodes;
private:
    void clean();
public:
    MultiNode();
    ~MultiNode();

    void addNode(scoped<INode> node, NodeType type);

    virtual bool closed() const override;
    virtual weak<ITask> updateTask() override;
    virtual weak<ITask> renderTask() override;
    virtual weak<ITask> syncTask() override;
    virtual weak<ITask> dispatchTask() override;
};

}}


/*****************************************************************************/
#endif
