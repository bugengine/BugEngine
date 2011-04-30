/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#include    <posix/platformrenderer.hh>
#include    <X11/X.h>
#include    <X11/Xlib.h>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window::PlatformWindow : public minitl::refcountable
{
    friend class Window;
private:
    ::Display*      m_display;
    ::Window        m_window;
public:
    PlatformWindow(::Display* display, ::Window window);
    ~PlatformWindow();
};

Window::PlatformWindow::PlatformWindow(::Display* display, ::Window window)
:   m_display(display)
,   m_window(window)
{
}

Window::PlatformWindow::~PlatformWindow()
{
    XDestroyWindow(m_display, m_window);
}



Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   IRenderTarget(renderer)
,   m_window(scoped<PlatformWindow>::create(gameArena(), renderer->m_platformRenderer->m_display, renderer->m_platformRenderer->createWindow(flags)))
{
    Window* w = this;
    XChangeProperty(renderer->m_platformRenderer->m_display, m_window->m_window, renderer->m_platformRenderer->m_windowProperty,
                    XA_INTEGER, 8, PropModeReplace, (unsigned char *)&w, sizeof(w));
}

Window::~Window()
{
    close();
}

void Window::close()
{
    m_window = scoped<PlatformWindow>();
}

bool Window::closed() const
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

