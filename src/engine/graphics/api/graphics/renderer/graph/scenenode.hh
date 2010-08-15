/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_SCENENODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_SCENENODE_HH_
/*****************************************************************************/
#include    <graphics/renderer/graph/inode.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine { namespace Graphics
{

class IScene;
class IRenderTarget;

class SceneNode : public INode
{
private:
    ref<TaskGroup>                  m_renderTask;
    ref<ITask>                      m_dispatchTask;
    ref<IScene>                     m_scene;
    ref<IRenderTarget>              m_renderTarget;
    TaskGroup::TaskStartConnection  m_renderStartTask;
    TaskGroup::TaskEndConnection    m_renderEndTask;
    AsyncDispatchJobGraph           m_jobGraph;
private:
    void dispatch();
public:
    SceneNode(ref<IScene> scene, ref<IRenderTarget> renderTarget);
    ~SceneNode();

    virtual bool closed() const override;
    virtual weak<ITask> updateTask() override;
    virtual weak<ITask> renderTask() override;
    virtual weak<ITask> syncTask() override;
    virtual weak<ITask> dispatchTask() override;
};

}}

/*****************************************************************************/
#endif
