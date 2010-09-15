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
:   IRenderTarget(renderer)
{
    RECT size;
    size.left   = 0;
    size.right  = flags.size.x();
    size.top    = 0;
    size.bottom = flags.size.y();
    if(flags.border)
        AdjustWindowRect(&size, WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE);
    m_window = CreateWindowEx( !flags.border ? WS_EX_TOPMOST : 0,
        renderer->getWindowClassName().c_str(),
        flags.title.c_str(),
        flags.border ? WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX : WS_POPUP,
        flags.position.x(), flags.position.y(),
        size.right-size.left, size.bottom-size.top,
        NULL, NULL, hDllInstance, NULL );
    ShowWindow(m_window, SW_SHOW);
    UpdateWindow(m_window);
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
        DestroyWindow(hWnd);
}

uint2 Window::getDimensions() const
{
    RECT r;
    GetClientRect(m_window, &r);
    return int2(r.right-r.left, r.bottom-r.top);
}

}}}
