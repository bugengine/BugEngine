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
    ref<IScene>                     m_scene;
    ref<IRenderTarget>              m_renderTarget;
    ref<TaskGroup>                  m_task;
    TaskGroup::TaskStartConnection  m_startUpdateConnection;
    TaskGroup::TaskEndConnection    m_endUpdateConnection;
    ITask::CallbackConnection       m_callbackConnection;
public:
    SceneNode(ref<IScene> scene, ref<IRenderTarget> renderTarget);
    ~SceneNode();

    virtual weak<ITask> renderTask() override;
    virtual bool closed() const override;
};

}}

/*****************************************************************************/
#endif
