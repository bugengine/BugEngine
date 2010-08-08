/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>
#include    <GL/gl.h>


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
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::close()
{
    Windowing::Window::close();
}

void Window::begin(ClearMode /*clear*/)
{
    setCurrent();
}

void Window::end(PresentMode present)
{
    OutputDebugString("begin render OpenGL\n");
    Thread::sleep(50);
    OutputDebugString("end render OpenGL\n");
    if(present == Present)
    {
#ifdef BE_PLATFORM_WIN32
        //SwapBuffers(m_dc);
#else
        glXSwapBuffers(be_checked_cast<Renderer>(m_renderer)->m_display, m_window);
#endif
    }
}

}}}
