/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <core/threads/event.hh>
#include    <windows.h>

#pragma warning(disable:4311 4312 4355)

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace Windowing
{


struct WindowCreationFlags
{
    const char *className;
    const char *title;
    int x;
    int y;
    RECT size;
    DWORD flags;
    bool fullscreen;
};


#define WM_BE_CREATEWINDOW  0
#define WM_BE_DESTROYWINDOW 1

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
        
        switch( msg )
        {
            case WM_CLOSE:
                {
                    Window* win = (Window*)(GetWindowLongPtr(hWnd,GWLP_USERDATA));
                    be_assert(win, "BugEngine window not associated with hWnd");
                    win->close();
                    break;
                }

            case WM_CREATE:
                {
                    nbWindows++;
                    break;
                }

            case WM_DESTROY:
                be_assert(nbWindows, "trying to destroy a window but no window was registered");
                nbWindows--;
                break;

            case WM_PAINT:
                {
                    ValidateRect(hWnd, NULL);
                    break;
                }


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
:   m_windowClassName(minitl::format<>("__be__%p__") | (const void*)this)
,   m_windowManagementThread("WindowManagement", &Renderer::updateWindows, (intptr_t)this, 0, Thread::AboveNormal)
{
    memset(&m_wndClassEx, 0, sizeof(WNDCLASSEX));
    m_wndClassEx.lpszClassName  = m_windowClassName.c_str();
    m_wndClassEx.cbSize         = sizeof(WNDCLASSEX);
    m_wndClassEx.style          = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;
    m_wndClassEx.lpfnWndProc    = WindowProc;
    m_wndClassEx.hInstance      = hDllInstance;
    m_wndClassEx.hIcon          = LoadIcon(hDllInstance, (LPCTSTR)IDI_BE_ICON);
    m_wndClassEx.hIconSm        = 0;
    m_wndClassEx.hCursor        = LoadCursor(0, (LPCTSTR)IDC_ARROW);
    m_wndClassEx.hbrBackground  = NULL;
    m_wndClassEx.lpszMenuName   = NULL;
    m_wndClassEx.cbClsExtra     = 0;
    m_wndClassEx.cbWndExtra     = sizeof(Window*);

    RegisterClassEx(&m_wndClassEx);
}

Renderer::~Renderer()
{
    postMessage(WM_QUIT, 0, 0);
    UnregisterClass(m_windowClassName.c_str(), hDllInstance);
}

HWND Renderer::createWindowImplementation(const WindowCreationFlags* flags) const
{
    WindowCreationEvent event;
    event.flags = flags;
    postMessage(WM_USER+WM_BE_CREATEWINDOW, (WPARAM)&event, 0);
    event.event.wait();
    return event.hWnd;
}

void Renderer::destroyWindowImplementation(HWND hWnd)
{
    postMessage(WM_USER+WM_BE_DESTROYWINDOW, (WPARAM)hWnd, 0);
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

UINT Renderer::messageCount() const
{
    return 2;
}

void Renderer::postMessage(UINT msg, WPARAM wParam, LPARAM lParam) const
{
    PostThreadMessageA((DWORD)m_windowManagementThread.id(), msg, wParam, lParam);
}

void Renderer::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    be_forceuse(wParam);
    be_forceuse(lParam);
    switch(msg)
    {
    case WM_USER+WM_BE_CREATEWINDOW:
        {
            WindowCreationEvent* event = (WindowCreationEvent*)wParam;
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

    case WM_USER+WM_BE_DESTROYWINDOW:
        {
            HWND hWnd = (HWND)wParam;
            DestroyWindow(hWnd);
        }
        break;
    default:
        be_assert(false, "unhandled message type %d" | msg);
        break;
    }
}

void Renderer::flush()
{
}

intptr_t Renderer::updateWindows(intptr_t p1, intptr_t /*p2*/)
{
    weak<Renderer> renderer(reinterpret_cast<Renderer*>(p1));
    MSG msg;
    while(::GetMessage(&msg, 0, 0, 0))
    {
        if(msg.message >= WM_USER && msg.message < WM_APP)
        {
            renderer->handleMessage(msg.message, msg.wParam, msg.lParam);
        }
        else
        {
            DispatchMessage(&msg);
        }
    }
    return 0;
}

}}}
