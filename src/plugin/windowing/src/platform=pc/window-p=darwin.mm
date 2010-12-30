/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window::PlatformWindow : public minitl::refcountable
{
    friend class Window;
private:
public:
    PlatformWindow();
    ~PlatformWindow();
};

Window::PlatformWindow::PlatformWindow()
{
}

Window::PlatformWindow::~PlatformWindow()
{
}



Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   IRenderTarget(renderer)
,   m_window(scoped<PlatformWindow>::create(gameArena()))
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
        m_window = scoped<PlatformWindow>();
    }
}

bool Window::isClosed() const
{
    return m_window == 0;
}

void* Window::getWindowHandle() const
{
    return (void*)0;
}



uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}

