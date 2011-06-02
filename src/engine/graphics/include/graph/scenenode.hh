/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_SCENENODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_SCENENODE_HH_
/*****************************************************************************/
#include    <graph/inode.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine { namespace Graphics
{

class IScene;
class IRenderTarget;

class SceneNode : public INode
{
private:
    ref<ITask>                  m_renderTask;
    weak<IScene>                m_scene;
    weak<IRenderTarget>         m_renderTarget;
    ITask::CallbackConnection   m_startUpdateConnection;
    ITask::CallbackConnection   m_startRenderConnection;
    ITask::CallbackConnection   m_startFlush;
    ITask::CallbackConnection   m_startRender;
private:
    void render();
public:
    SceneNode(weak<IScene> scene, weak<IRenderTarget> renderTarget);
    ~SceneNode();

    virtual weak<ITask> updateTask() override;
    virtual weak<ITask> renderTask() override;
};

}}

/*****************************************************************************/
#endif
