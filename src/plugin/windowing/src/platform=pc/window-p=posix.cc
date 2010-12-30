/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window::PlatformWindow : public minitl::refcountable
{
    friend class Window;
private:
    ::Window        m_window;
public:
    PlatformWindow(::Window window);
    ~PlatformWindow();
};

Window::PlatformWindow::PlatformWindow(::Window window)
:   m_window(window)
{
}

Window::PlatformWindow::~PlatformWindow()
{
}



Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   IRenderTarget(renderer)
,   m_window(scoped<PlatformWindow>::create(gameArena(), renderer->m_platformRenderer->createWindow(flags)))
{
    Window* w = this;
    XChangeProperty(renderer->m_platformRenderer->m_display, m_window->m_window, renderer->m_platformRenderer->m_windowProperty,
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
        ::Window bu = m_window->m_window;
        m_window = scoped<PlatformWindow>();
        XDestroyWindow(be_checked_cast<Renderer>(m_renderer)->m_platformRenderer->m_display, bu);
    }
}

bool Window::isClosed() const
{
    return m_window == 0;
}

void* Window::getWindowHandle() const
{
    be_assert_recover(m_window, "no window implementation is created", return 0);
    return (void*)&m_window->m_window;
}



uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}

