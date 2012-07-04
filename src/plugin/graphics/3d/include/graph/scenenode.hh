/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_SCENENODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_SCENENODE_HH_
/*****************************************************************************/
#include    <graph/inode.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine
{

namespace World
{
class World;
}
class IRenderTarget;

class SceneNode : public INode
{
private:
    weak<const BugEngine::World::World> m_world;
    ref<Task::ITask>                    m_renderTask;
    ref<Task::ITask>                    m_dispatchTask;
    weak<IRenderTarget>                 m_renderTarget;
    Task::ITask::CallbackConnection     m_startRender;
    Task::ITask::CallbackConnection     m_startDispatch;
    Task::ITask::CallbackConnection     m_startFlush;
    Task::ITask::CallbackConnection     m_waitOnRender;
    Task::ITask::CallbackConnection     m_waitOnFlush;
private:
    void render();
    void dispatch();
public:
    SceneNode(weak<IRenderTarget> renderTarget, weak<const BugEngine::World::World> world);
    ~SceneNode();

    virtual weak<Task::ITask> updateTask() override;
    virtual weak<Task::ITask> dispatchTask() override;
};

}

/*****************************************************************************/
#endif
