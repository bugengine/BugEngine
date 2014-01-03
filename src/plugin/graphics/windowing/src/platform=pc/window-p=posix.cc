/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#include    <posix/platformrenderer.hh>
#include    <X11/X.h>
#include    <X11/Xlib.h>
#include    <X11/Xatom.h>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine { namespace Windowing
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
    m_window.reset(scoped<PlatformWindow>::create(m_renderer->arena(),
                                              be_checked_cast<const Renderer>(m_renderer)->m_platformRenderer->m_platformData.display,
                                              be_checked_cast<const Renderer>(m_renderer)->m_platformRenderer->createWindow(0, 0, 200, 200)));
    Window* w = this;
    XChangeProperty(be_checked_cast<const Renderer>(m_renderer)->m_platformRenderer->m_platformData.display,
                    m_window->m_window,
                    be_checked_cast<const Renderer>(m_renderer)->m_platformRenderer->m_windowProperty,
                    XA_INTEGER, 8, PropModeReplace, (unsigned char *)&w, sizeof(w));
}

void Window::unload()
{
    m_window.reset(scoped<PlatformWindow>());
}

void* Window::getWindowHandle() const
{
    be_assert_recover(m_window, "no window implementation is created", return 0);
    return (void*)&m_window->m_window;
}

uint2 Window::getDimensions() const
{
    return make_uint2(1920, 1200);
}

}}
