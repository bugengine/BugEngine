/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#include    <darwin/platformrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine { namespace Windowing
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



Window::Window(weak<const RenderWindowDescription> renderWindowDescription, weak<const Renderer> renderer)
:   IRenderTarget(renderWindowDescription, renderer)
,   m_window()
{
}

Window::~Window()
{
}

void Window::load(weak<const Resource::Description> /*renderWindowDescription*/)
{
    uint2 dimensions = make_uint2(800, 600); //be_checked_cast<const RenderWindow>(resource)->dimensions;
    m_window.reset(scoped<PlatformWindow>::create(m_renderer->arena(), dimensions[0], dimensions[1]));
}

void Window::unload()
{
    m_window.reset(scoped<PlatformWindow>());
}
	

void* Window::getWindowHandle() const
{
    be_assert_recover(m_window, "no window implementation is created", return 0);
    return (void*)m_window->m_window;
}

uint2 Window::getDimensions() const
{
    return make_uint2(1920, 1200);
}

}}
