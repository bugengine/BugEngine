/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

Renderer::~Renderer()
{
    cgDestroyContext(m_context);
}

void Renderer::attachWindow(Window* w)
{
}

}}}
