/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   Windowing::Window(renderer, flags)
{
    renderer->attachWindow(this);
}

Window::~Window()
{
}

bool Window::closed() const
{
    return m_window == 0;
}

void Window::setCurrent()
{
#ifdef BE_PLATFORM_WIN32
    wglMakeCurrent(m_dc, be_checked_cast<Renderer>(m_renderer)->m_glContext);
#else
    glXMakeCurrent(be_checked_cast<Renderer>(m_renderer)->m_display, m_window, be_checked_cast<Renderer>(m_renderer)->m_glContext);
#endif
}

void Window::close()
{
    Windowing::Window::close();
}

void Window::begin()
{
    setCurrent();
    uint2 size = getDimensions();
}

void Window::end()
{
#ifdef BE_PLATFORM_WIN32
    SwapBuffers(m_dc);
#else
    glXSwapBuffers(be_checked_cast<Renderer>(m_renderer)->m_display, m_window);
#endif
}

}}}
