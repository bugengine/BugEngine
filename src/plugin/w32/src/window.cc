/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <win32/stdafx.h>
#include    <win32/window.hh>
#include    <win32/renderer.hh>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace Win32
{

Window::Window(Renderer* renderer, WindowFlags flags, const Scene* scene)
:   RenderTarget(scene)
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
