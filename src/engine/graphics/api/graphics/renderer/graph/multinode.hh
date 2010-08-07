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
private:
    struct NodeInfo
    {
        scoped<INode>                      node;
        TaskGroup::TaskEndConnection    groupConnection;
        ITask::CallbackConnection       startConnection;
        NodeInfo(scoped<INode> node, ref<ITask> updateTask, ref<TaskGroup> owner);
        NodeInfo(const NodeInfo& other);
    private:
        NodeInfo& operator=(const NodeInfo& other);
    };
    ref<TaskGroup>                  m_task;
    ref<ITask>                      m_updateTask;
    TaskGroup::TaskStartConnection  m_startConnection;
    TaskGroup::TaskEndConnection    m_endConnection;
    minitl::list< NodeInfo >        m_mainNodes;
    minitl::list< NodeInfo >        m_secondaryNodes;
private:
    void update();
public:
    MultiNode();
    ~MultiNode();

    void addMainNode(scoped<INode> node);
    void addSecondaryNode(scoped<INode> node);

    virtual weak<ITask> renderTask() override;
    virtual bool closed() const override;
};

}}

/*****************************************************************************/
#endif
