/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

Renderer::~Renderer()
{
}

void Renderer::attachWindow(Window* w)
{
    if(!m_glContext)
    {
        glXCreateContext(m_display, m_visual, 0, True);
    }
    w->m_glContext = glXCreateContext(m_display, m_visual, m_glContext, True);
}

}}}

