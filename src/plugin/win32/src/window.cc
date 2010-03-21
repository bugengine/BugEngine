/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <win32/stdafx.h>
#include    <win32/window.hh>
#include    <win32/renderer.hh>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace Win32
{

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   RenderTarget()
,   m_renderer(renderer)
{
    WindowCreationFlags f;
    f.className = renderer->getWindowClassName().c_str();
    f.title = flags.title.c_str();
    f.flags = flags.border ? WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX : WS_POPUP;
    f.x = flags.position.x();
    f.y = flags.position.y();
    f.size.left   = 0;
    f.size.right  = flags.size.x();
    f.size.top    = 0;
    f.size.bottom = flags.size.y();
    f.fullscreen = !flags.border;
    if(flags.border)
        AdjustWindowRect(&f.size, WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE);
    m_window = renderer->createWindowImplementation(&f);
    m_dc = GetDC(m_window);
    if(!m_window)
    {
        BE_WIN32_PRINTERROR();
    }
    SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG)this);
}

Window::~Window()
{
    close();
}

void Window::close()
{
    HWND hWnd = m_window;
    m_window = 0;
    if(hWnd)
        m_renderer->destroyWindowImplementation(hWnd);
}

uint2 Window::getDimensions() const
{
    RECT r;
    GetClientRect(m_window, &r);
    return int2(r.right-r.left, r.bottom-r.top);
}

}}}
