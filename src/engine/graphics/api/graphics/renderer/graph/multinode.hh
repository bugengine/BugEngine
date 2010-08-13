/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_MULTINODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_MULTINODE_HH_
/*****************************************************************************/
#include    <graphics/renderer/graph/inode.hh>
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
        TaskGroup::TaskStartConnection  dispatchStartConnection;
        TaskGroup::TaskEndConnection    dispatchEndConnection;
        ITask::CallbackConnection       chainFlush;
        NodeType                        type;
        NodeInfo(scoped<INode> node, ref<ITask> updateTask, ref<TaskGroup> render, ref<TaskGroup> dispatch, NodeType type);
        NodeInfo(const NodeInfo& other);
        void disconnect();
    private:
        NodeInfo& operator=(const NodeInfo& other);
    };
    ref<TaskGroup>                  m_globalTask;
    ref<TaskGroup>                  m_renderTask;
    ref<TaskGroup>                  m_dispatchTask;
    ref<ITask>                      m_updateTask;
    TaskGroup::TaskStartConnection  m_startUpdateConnection;
    TaskGroup::TaskEndConnection    m_endUpdateConnection;
    ITask::CallbackConnection       m_startRender;
    ITask::CallbackConnection       m_startDispatch;
    minitl::list< NodeInfo >        m_nodes;
    u32                             m_mainNodes;
private:
    void update();
public:
    MultiNode();
    ~MultiNode();

    void addNode(scoped<INode> node, NodeType type);

    virtual weak<ITask> renderTask() override;
    virtual weak<ITask> dispatchTask() override;
    virtual bool closed() const override;
    virtual void disconnect() override;
};

}}

/*****************************************************************************/
#endif
