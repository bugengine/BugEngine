/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/window.hh>
#include    <X/renderer.hh>

namespace BugEngine { namespace Graphics { namespace X
{

Window::Window(Renderer* renderer, WindowFlags flags, const Scene* scene)
:   RenderTarget(scene)
,   m_renderer(renderer)
{
}

Window::~Window()
{
	close();
}

void Window::close()
{
}

uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}
