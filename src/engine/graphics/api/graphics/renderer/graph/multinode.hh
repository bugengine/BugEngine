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
        ref<INode>                      node;
        TaskGroup::TaskEndConnection    groupConnection;
        ITask::CallbackConnection       startConnection;
        NodeInfo(ref<INode> node, ref<ITask> updateTask, ref<TaskGroup> owner);
        NodeInfo(const NodeInfo& other);
        NodeInfo& operator=(const NodeInfo& other);
    };
    ref<TaskGroup>                  m_task;
    ref<ITask>                      m_updateTask;
    TaskGroup::TaskStartConnection  m_startConnection;
    TaskGroup::TaskEndConnection    m_endConnection;
    minitl::vector< NodeInfo >      m_mainNodes;
    minitl::vector< NodeInfo >      m_secondaryNodes;
private:
    void update();
public:
    MultiNode();
    ~MultiNode();

    void addMainNode(ref<INode> node);
    void addSecondaryNode(ref<INode> node);

    virtual weak<ITask> renderTask() override;
    virtual bool closed() const override;
};

}}

/*****************************************************************************/
#endif
