/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#include    <darwin/platformrenderer.hh>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window::PlatformWindow : public minitl::refcountable
{
    friend class Window;
private:
    NSWindow*   m_window;
public:
    PlatformWindow(WindowFlags flags);
    ~PlatformWindow();
};

Window::PlatformWindow::PlatformWindow(WindowFlags flags)
    :   m_window([[NSWindow alloc] initWithContentRect:NSMakeRect(flags.position.x(), flags.position.y(), flags.size.x(), flags.size.y())
                                             styleMask:NSTitledWindowMask | NSResizableWindowMask
                                               backing:NSBackingStoreBuffered
                                                 defer:NO])
{
}

Window::PlatformWindow::~PlatformWindow()
{
}



Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   IRenderTarget(renderer)
,   m_window(scoped<PlatformWindow>::create(gameArena(), flags))
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

bool Window::closed() const
{
    return m_window == 0;
}

void* Window::getWindowHandle() const
{
    return (void*)m_window->m_window;
}

uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}

