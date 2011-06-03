/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#include    <win32/platformrenderer.hh>
#include    <win32/platformwindow.hh>
#include    <graphics/objects/rendertarget.script.hh>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace Windowing
{

Window::PlatformWindow::PlatformWindow(weak<const Renderer> renderer, weak<Window> window)
:   m_renderer(renderer)
{
    WindowCreationFlags f;
    f.title = "TODO";
    f.flags = /*TODO*/ WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    f.x = 0;
    f.y = 0;
    f.size.left   = 0;
    f.size.right  = 800;
    f.size.top    = 0;
    f.size.bottom = 600;
    f.fullscreen = false;
    AdjustWindowRect(&f.size, WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE);
    m_window = renderer->m_platformRenderer->createWindowImplementation(f);
    if (!m_window)
    {
        BE_WIN32_PRINTERROR();
    }
    SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)window.operator->());
    ShowWindow(m_window, SW_SHOW);
    UpdateWindow(m_window);
}

Window::PlatformWindow::~PlatformWindow()
{
    HWND hWnd = m_window;
    m_window = 0;
    if (hWnd)
        m_renderer->m_platformRenderer->destroyWindowImplementation(hWnd);
}

Window::Window(weak<const RenderTarget> resource, weak<Renderer> renderer)
:   IRenderTarget(resource, renderer)
,   m_window(scoped<PlatformWindow>())
{
}

Window::~Window()
{
    close();
}

void Window::load(weak<const Resource> resource)
{
    m_window = scoped<PlatformWindow>::create(m_renderer->arena(), m_renderer, this);
}

void Window::unload()
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

uint2 Window::getDimensions() const
{
    RECT r;
    GetClientRect(m_window->m_window, &r);
    return int2(r.right-r.left, r.bottom-r.top);
}

void* Window::getWindowHandle() const
{
    be_assert_recover(m_window, "no window implementation is created", return 0);
    return (void*)&m_window->m_window;
}

}}}
