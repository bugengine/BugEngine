/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/irenderer.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine { namespace Graphics
{

IRenderer::IRenderer()
:   m_syncTask(ref< Task< MethodCaller<IRenderer, &IRenderer::flush> > >::create<Arena::General>("flush", color32(255,0,0),  MethodCaller<IRenderer, &IRenderer::flush>(this), Scheduler::High))
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
