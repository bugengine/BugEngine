/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/irenderer.hh>
#include    <system/scheduler/task/method.hh>

#include    <graph/inode.hh>
#include    <graph/multinode.hh>
#include    <graph/scenenode.hh>
#include    <graph/loader.hh>

namespace BugEngine { namespace Graphics
{

IRenderer::IRenderer()
    :   m_syncTask(ref< Task< MethodCaller<IRenderer, &IRenderer::flush> > >::create(taskArena(), "flush", color32(255,0,0),  MethodCaller<IRenderer, &IRenderer::flush>(this), Scheduler::High, Scheduler::MainThread))
    ,   m_sceneLoader(scoped<SceneGraphLoader>::create(gameArena()))
{
}

IRenderer::~IRenderer()
{
}

weak<ITask> IRenderer::syncTask()
{
    return m_syncTask;
}

}}
