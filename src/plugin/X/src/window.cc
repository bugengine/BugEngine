/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/window.hh>
#include    <X/renderer.hh>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Graphics { namespace X
{

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   IRenderTarget(renderer)
,   m_window(renderer->createWindow(flags))
{
    Window* w = this;
    XChangeProperty(renderer->m_display, m_window, renderer->m_windowProperty,
                    XA_INTEGER, 32, PropModeReplace, (const unsigned char*)&w, sizeof(w)/4);
    be_info("%p" | (const void*)w);
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
        XDestroyWindow(be_checked_cast<Renderer>(m_renderer)->m_display, bu);
    }
}

uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}

