/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <win32/platformrenderer.hh>
#include    <core/threads/event.hh>
#pragma warning(disable:4311 4312 4355)

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace Windowing
{

#define WM_BE_CREATEWINDOW  0
#define WM_BE_DESTROYWINDOW 1
#define WM_BE_CREATECONTEXT 2
#define WM_BE_DESTROYCONTEXT 3

namespace
{
    struct WindowCreationEvent
    {
        const WindowCreationFlags* flags;
        HWND                 hWnd;
        Event                event;
    };
    struct ContextCreationEvent
    {
        void*   params;
        Event   event;
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

intptr_t Renderer::PlatformRenderer::updateWindows(intptr_t p1, intptr_t /*p2*/)
{
    weak<Renderer::PlatformRenderer> renderer(reinterpret_cast<Renderer::PlatformRenderer*>(p1));
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

Renderer::PlatformRenderer::PlatformRenderer(weak<Renderer> renderer)
:   m_renderer(renderer)
,   m_windowClassName(minitl::format<>("__be__%p__") | (const void*)this)
,   m_windowManagementThread("WindowManagement", &Renderer::PlatformRenderer::updateWindows, (intptr_t)this, 0, Thread::AboveNormal)
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

Renderer::PlatformRenderer::~PlatformRenderer()
{
    postMessage(WM_QUIT, 0, 0);
    UnregisterClass(m_windowClassName.c_str(), hDllInstance);
}

void Renderer::PlatformRenderer::postMessage(UINT msg, WPARAM wParam, LPARAM lParam) const
{
    PostThreadMessageA((DWORD)m_windowManagementThread.id(), msg, wParam, lParam);
}

void Renderer::PlatformRenderer::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
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
    case WM_USER+WM_BE_CREATECONTEXT:
        {
            ContextCreationEvent* event = (ContextCreationEvent*)wParam;
            m_renderer->createContext(event->params);
            event->event.set();
        }
        break;
    case WM_USER+WM_BE_DESTROYCONTEXT:
        {
            ContextCreationEvent* event = (ContextCreationEvent*)wParam;
            m_renderer->destroyContext(0);
            event->event.set();
        }
        break;
    default:
        be_assert(false, "unhandled message type %d" | msg);
        break;
    }
}

HWND Renderer::PlatformRenderer::createWindowImplementation(const WindowCreationFlags* flags) const
{
    WindowCreationEvent event;
    event.flags = flags;
    postMessage(WM_USER+WM_BE_CREATEWINDOW, (WPARAM)&event, 0);
    event.event.wait();
    return event.hWnd;
}

const istring& Renderer::PlatformRenderer::getWindowClassName() const
{
    return m_windowClassName;
}

void Renderer::PlatformRenderer::destroyWindowImplementation(HWND hWnd)
{
    postMessage(WM_USER+WM_BE_DESTROYWINDOW, (WPARAM)hWnd, 0);
}

//-----------------------------------------------------------------------------

Renderer::Renderer()
    :   m_platformRenderer(scoped<PlatformRenderer>::create<Arena::General>(this))
{
}

Renderer::~Renderer()
{
}

void Renderer::flush()
{
}

uint2 Renderer::getScreenSize()
{
    RECT rect;
    GetWindowRect(GetDesktopWindow(),&rect);
    return uint2(rect.right - rect.left, rect.bottom - rect.top);
}

void Renderer::createContextAsync(void* params)
{
    ContextCreationEvent event;
    event.params = params;
    m_platformRenderer->postMessage(WM_USER+WM_BE_CREATECONTEXT, (WPARAM)&event, 0);
    event.event.wait();
    return;
}

void Renderer::destroyContextAsync(void* params)
{
    ContextCreationEvent event;
    m_platformRenderer->postMessage(WM_USER+WM_BE_DESTROYCONTEXT, (WPARAM)&event, 0);
    event.event.wait();
    return;
}

void Renderer::createContext(void* params)
{
}

void Renderer::destroyContext(void* params)
{
}

}}}
