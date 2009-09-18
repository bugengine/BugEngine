/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/renderer.hh>
#include    <X/window.hh>
#include    <core/threads/event.hh>

namespace BugEngine { namespace Graphics { namespace X
{

namespace
{
}

Renderer::Renderer()
:   m_windowManagementThread("WindowManagement", &Renderer::updateWindows, (intptr_t)&m_exit, 0, Thread::AboveNormal)
,   m_exit(false)
{
}

Renderer::~Renderer()
{
}

uint2 Renderer::getScreenSize()
{
    return uint2(1920, 1200);
}

int Renderer::step() const
{
    return (int)m_exit;
}

intptr_t Renderer::updateWindows(intptr_t p1, intptr_t p2)
{
    return 0;
}

}}}
