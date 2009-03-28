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
#include    <win32/renderer.hh>
#include    <win32/window.hh>
#include    <core/threads/event.hh>

#pragma warning(disable:4311 4312 4355)

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace Win32
{

#define WM_BE_CREATEWINDOW  WM_USER
#define WM_BE_DESTROYWINDOW WM_USER+1

namespace
{
    struct WindowCreationEvent
    {
        const WindowCreationFlags* flags;
        HWND                 hWnd;
        Event                event;
    };

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        static unsigned int nbWindows = 0;
        static int* memoryTest = 0;
        
        switch( msg )
        {
            case WM_CLOSE:
                {
                    Window* win = (Window*)(GetWindowLongPtr(hWnd,GWLP_USERDATA));
                    Assert(win);
                    win->close();
                    break;
                }

            case WM_CREATE:
                {
                    nbWindows++;
                    Assert(nbWindows);
                    break;
                }

            case WM_DESTROY:
                Assert(nbWindows);
                nbWindows--;
                if(!nbWindows)
                    PostQuitMessage(0);
                break;

            case WM_SETFOCUS:
                break;
            case WM_KILLFOCUS:
                break;
            default:
                return DefWindowProc( hWnd, msg, wParam, lParam );
        }

        return 0;
    }
}

Renderer::Renderer()
:   m_windowClassName(minitl::format<>("__be__%p__") | (void*)this)
,   m_windowManagementThread("WindowManagement", &Renderer::updateWindows, (intptr_t)&m_exit, 0, Thread::AboveNormal)
{
    memset(&m_wndClassEx, 0, sizeof(WNDCLASSEX));
    m_wndClassEx.lpszClassName  = m_windowClassName.c_str();
    m_wndClassEx.cbSize         = sizeof(WNDCLASSEX);
    m_wndClassEx.style          = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;
    m_wndClassEx.lpfnWndProc    = WindowProc;
    m_wndClassEx.hInstance      = hDllInstance;
    m_wndClassEx.hIcon          = 0;
    m_wndClassEx.hIconSm        = 0;
    m_wndClassEx.hCursor        = LoadCursor(0, (LPCTSTR)IDC_ARROW);
    m_wndClassEx.hbrBackground  = NULL;
    m_wndClassEx.lpszMenuName   = NULL;
    m_wndClassEx.cbClsExtra     = 0;
    m_wndClassEx.cbWndExtra     = sizeof(Window*);

    BE_WIN32_CHECKRESULT(RegisterClassEx(&m_wndClassEx));
}

Renderer::~Renderer()
{
    BE_WIN32_CHECKRESULT(UnregisterClass(m_windowClassName.c_str(), hDllInstance));
}

HWND Renderer::createWindowImplementation(const WindowCreationFlags* flags) const
{
    WindowCreationEvent event;
    event.flags = flags;
    PostThreadMessageA(m_windowManagementThread.id(), WM_BE_CREATEWINDOW, (WPARAM)&event, 0);
    event.event.wait();
    return event.hWnd;
}

void Renderer::destroyWindowImplementation(HWND hWnd)
{
    PostThreadMessageA(m_windowManagementThread.id(), WM_BE_DESTROYWINDOW, (WPARAM)hWnd, 0);
}

uint2 Renderer::getScreenSize()
{
    RECT rect;
    GetWindowRect(GetDesktopWindow(),&rect);
    return uint2(rect.right - rect.left, rect.bottom - rect.top);
}

const istring& Renderer::getWindowClassName() const
{
    return m_windowClassName;
}

int Renderer::step() const
{
    return (int)m_exit;
}

intptr_t Renderer::updateWindows(intptr_t p1, intptr_t p2)
{
    MSG msg;
    bool& shouldQuit = *(bool*)p1;
    shouldQuit = false;
    while(::GetMessage(&msg, 0, 0, 0))
    {
        switch(msg.message)
        {
        case WM_BE_CREATEWINDOW:
            {
                WindowCreationEvent* event = (WindowCreationEvent*)msg.wParam;
                event->hWnd = CreateWindowEx( event->flags->fullscreen ? WS_EX_TOPMOST : 0,
                    event->flags->className,
                    event->flags->title,
                    event->flags->flags,
                    event->flags->x, event->flags->y,
                    event->flags->size.right-event->flags->size.left, event->flags->size.bottom-event->flags->size.top,
                    NULL, NULL, hDllInstance, NULL );
                ShowWindow(event->hWnd, SW_SHOW);
                UpdateWindow(event->hWnd);
                event->event.set();
            }
            break;

        case WM_BE_DESTROYWINDOW:
            {
                HWND hWnd = (HWND)msg.wParam;
                DestroyWindow(hWnd);
            }
            break;
        }
        DispatchMessage(&msg);
    }
    shouldQuit = true;
    return 0;
}

}}}
