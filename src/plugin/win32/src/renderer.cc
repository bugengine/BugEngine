/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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

namespace
{

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
    UnregisterClass(m_windowClassName.c_str(), hDllInstance);
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

void Renderer::flush()
{
    MSG msg;
    while(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE|PM_NOYIELD))
    {
        DispatchMessage(&msg);
    }
    return;
}

}}}
