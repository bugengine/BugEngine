/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/graph/scenenode.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/scene/iscene.hh>

namespace BugEngine { namespace Graphics
{

SceneNode::SceneNode(ref<IScene> scene, ref<IRenderTarget> renderTarget)
:   m_scene(scene)
,   m_renderTarget(renderTarget)
,   m_task(ref<TaskGroup>::create("renderScene", color32(255, 0, 0)))
,   m_startUpdateConnection(m_task, m_scene->updateTask())
,   m_endUpdateConnection(m_task, m_renderTarget->flushTask())
,   m_callbackConnection(m_scene->updateTask(), m_renderTarget->flushTask()->startCallback())
{
}

SceneNode::~SceneNode()
{
}

weak<ITask> SceneNode::renderTask()
{
    return m_task;
}

bool SceneNode::closed() const
{
    return m_renderTarget->closed();
}

}}
