/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   Windowing::Window(renderer, flags)
,   m_owner(renderer)
{
    m_owner->attachWindow(this);
}

Window::~Window()
{
}

bool Window::closed() const
{
    be_unimplemented();
    return false;
}

void Window::setCurrent()
{
#ifdef BE_PLATFORM_WIN32
    wglMakeCurrent(m_dc, m_owner->m_glContext);
#else
    glXMakeCurrent(m_owner->m_display, m_window, m_owner->m_glContext);
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
    //m_owner->m_systemParams[Renderer::__Screen]->setValue(float4(be_checked_numcast<float>(size.x()), be_checked_numcast<float>(size.y())));
}

void Window::end()
{
#ifdef BE_PLATFORM_WIN32
    SwapBuffers(m_dc);
#else
    glXSwapBuffers(m_owner->m_display, m_window);
#endif
}

}}}
