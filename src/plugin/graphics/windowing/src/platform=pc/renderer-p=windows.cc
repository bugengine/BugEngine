/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <win32/platformrenderer.hh>
#include    <core/threads/event.hh>


namespace BugEngine { namespace Windowing
{

namespace
{
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch( msg )
        {
            case WM_CLOSE:
                {
                    //Window* win = (Window*)(GetWindowLongPtr(hWnd,GWLP_USERDATA));
                    //be_assert(win, "BugEngine window not associated with hWnd");
                    //win->close();
                    break;
                }

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

Renderer::PlatformRenderer::PlatformRenderer(weak<Renderer> renderer)
:   m_renderer(renderer)
,   m_windowClassName(minitl::format<128u>("__be__%p__") | (const void*)renderer)
{
    memset(&m_wndClassEx, 0, sizeof(WNDCLASSEX));
    m_wndClassEx.lpszClassName  = m_windowClassName.c_str();
    m_wndClassEx.cbSize         = sizeof(WNDCLASSEX);
    m_wndClassEx.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    m_wndClassEx.lpfnWndProc    = WindowProc;
    m_wndClassEx.hInstance      = (HINSTANCE)::GetModuleHandle(0);
    m_wndClassEx.hIcon          = LoadIcon((HINSTANCE)::GetModuleHandle(0), (LPCTSTR)IDI_BE_ICON);
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
    UnregisterClass(m_windowClassName.c_str(), (HINSTANCE)::GetModuleHandle(0));
}

HWND Renderer::PlatformRenderer::createWindowImplementation(const WindowCreationFlags& flags) const
{
    HWND hWnd = CreateWindowEx( flags.fullscreen ? WS_EX_TOPMOST : 0,
                                m_windowClassName.c_str(),
                                flags.title,
                                flags.flags,
                                flags.x, flags.y,
                                flags.size.right-flags.size.left, flags.size.bottom-flags.size.top,
                                NULL, NULL, (HINSTANCE)::GetModuleHandle(0), NULL );
    return hWnd;
}

const istring& Renderer::PlatformRenderer::getWindowClassName() const
{
    return m_windowClassName;
}

void Renderer::PlatformRenderer::destroyWindowImplementation(HWND hWnd)
{
    DestroyWindow(hWnd);
}

//-----------------------------------------------------------------------------

Renderer::Renderer(minitl::Allocator& allocator, weak<Resource::ResourceManager> manager)
    :   IRenderer(allocator, manager, Scheduler::MainThread)
    ,   m_platformRenderer(scoped<PlatformRenderer>::create(allocator, this))
{
}

Renderer::~Renderer()
{
}

bool Renderer::success() const
{
    return true;
}

void Renderer::flush()
{
    IRenderer::flush();
    MSG msg;
    while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        DispatchMessage(&msg);
    }
    return;
}

uint2 Renderer::getScreenSize() const
{
    RECT rect;
    GetWindowRect(GetDesktopWindow(),&rect);
    return make_uint2(rect.right - rect.left, rect.bottom - rect.top);
}

}}
