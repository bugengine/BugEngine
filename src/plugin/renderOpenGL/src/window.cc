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
,   m_closed(0)
{
    renderer->attachWindow(this);
}

Window::~Window()
{
#ifdef BE_PLATFORM_WIN32
    /*if(m_glContext)
    {
        wglDeleteContext(m_glContext);
        m_glContext = 0;
    }*/
#endif
}

bool Window::closed() const
{
    return m_closed > 0;
}

void Window::setCurrent()
{
#ifdef BE_PLATFORM_WIN32
    if(!wglMakeCurrent(m_dc, m_glContext))
    {
        char *errorMessage;
        ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ::GetLastError(),
            0,
            reinterpret_cast<LPTSTR>(&errorMessage),
            0,
            NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
#else
    if(m_window)
    {
        glXMakeCurrent(be_checked_cast<Renderer>(m_renderer)->m_display, m_window, be_checked_cast<Renderer>(m_renderer)->m_glContext);
    }
#endif
}

void Window::clearCurrent()
{
#ifdef BE_PLATFORM_WIN32
    if(!wglMakeCurrent(0, 0))
    {
        char *errorMessage;
        ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ::GetLastError(),
            0,
            reinterpret_cast<LPTSTR>(&errorMessage),
            0,
            NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
#else
    if(m_window)
    {
        glXMakeCurrent(be_checked_cast<Renderer>(m_renderer)->m_display, 0, 0);
    }
#endif
}

void Window::close()
{
    m_closed++;
}

void Window::begin(ClearMode clear)
{
    if(m_window)
    {
        setCurrent();
        if (clear == IRenderTarget::Clear)
        {
            glClearColor(0.3f, 0.3f, 03.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
}

void Window::end(PresentMode present)
{
    if(m_window)
    {
        glFlush();
        if(present == Present)
        {
    #ifdef BE_PLATFORM_WIN32
            SwapBuffers(m_dc);
    #else
            glXSwapBuffers(be_checked_cast<Renderer>(m_renderer)->m_display, m_window);
    #endif
        }
        clearCurrent();
        if(m_closed > 0 && m_window)
        {
            Windowing::Window::close();
            /*if(m_glContext)
            {
                wglDeleteContext(m_glContext);
                m_glContext = 0;
            }*/
        }
    }
}

}}}
