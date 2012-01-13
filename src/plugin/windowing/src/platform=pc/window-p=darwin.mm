/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#include    <darwin/platformrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window::PlatformWindow : public minitl::refcountable
{
    friend class Window;
private:
    NSWindow*   m_window;
public:
    PlatformWindow(u32 w, u32 h);
    ~PlatformWindow();
};

Window::PlatformWindow::PlatformWindow(u32 w, u32 h)
    :   m_window([[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, w, h)
                                             styleMask:NSTitledWindowMask | NSResizableWindowMask
                                               backing:NSBackingStoreBuffered
                                                 defer:NO])
{
}

Window::PlatformWindow::~PlatformWindow()
{
}



Window::Window(weak<const RenderWindow> resource, weak<const Renderer> renderer)
:   IRenderTarget(resource, renderer)
,   m_window()
{
}

Window::~Window()
{
}

void Window::load(weak<const Resource> /*resource*/)
{
    uint2 dimensions = uint2(800, 600); //be_checked_cast<const RenderWindow>(resource)->dimensions;
    m_window = scoped<PlatformWindow>::create(m_renderer->arena(), dimensions.x(), dimensions.y());
}

void Window::unload()
{
    m_window = scoped<PlatformWindow>();
}
	

void* Window::getWindowHandle() const
{
    be_assert_recover(m_window, "no window implementation is created", return 0);
    return (void*)m_window->m_window;
}

uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}

