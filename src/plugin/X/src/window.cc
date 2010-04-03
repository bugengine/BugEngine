/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/window.hh>
#include    <X/renderer.hh>

namespace BugEngine { namespace Graphics { namespace X
{

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   RenderTarget()
,   m_renderer(renderer)
,   m_window(renderer->createWindow(flags))
{
}

Window::~Window()
{
    close();
}

void Window::close()
{
    if(m_window)
    {
        ::Window bu = m_window;
        m_window = 0;
        XDestroyWindow(m_renderer->m_display, bu);
    }
}

uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}

