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
    struct RenderConfig
    {
        ref<TaskGroup>                  m_renderTask;
        TaskGroup::TaskStartConnection  m_renderStartTask;
        TaskGroup::TaskEndConnection    m_renderEndTask;
        RenderConfig(weak<SceneNode> node);
        void disconnect();
    };
    struct DispatchConfig
    {
        weak<ITask>                     m_syncTask;
        ref<ITask>                      m_realDispatch;
        ITask::CallbackConnection       m_startRealDispatch;
        ITask::CallbackConnection       m_waitForSync;
        DispatchConfig(weak<SceneNode> node);
        void disconnect();
    };
    friend struct RenderConfig;
    friend struct DispatchConfig;
private:
    ref<IScene>                     m_scene;
    ref<IRenderTarget>              m_renderTarget;
    RenderConfig                    m_renderConfig;
    DispatchConfig                  m_dispatchConfig;
    ITask::CallbackConnection       m_startDispatch;
    ITask::CallbackConnection       m_waitSync;
private:
    void dispatch();
public:
    SceneNode(ref<IScene> scene, ref<IRenderTarget> renderTarget);
    ~SceneNode();

    virtual weak<ITask> renderTask() override;
    virtual weak<ITask> dispatchTask() override;
    virtual bool closed() const override;
    virtual void disconnect() override;
};

}}

/*****************************************************************************/
#endif
