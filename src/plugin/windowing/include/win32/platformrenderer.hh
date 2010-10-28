/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_WIN32_PLATFORMRENDERER_HH_
#define BE_WINDOWING_WIN32_PLATFORMRENDERER_HH_
/*****************************************************************************/
#include    <core/threads/thread.hh>
#include    <windows.h>


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

class Renderer::PlatformRenderer : public minitl::refcountable
{
private:
    weak<Renderer>  m_renderer;
    istring         m_windowClassName;
    WNDCLASSEX      m_wndClassEx;
    Thread          m_windowManagementThread;
private:
    static intptr_t updateWindows(intptr_t p1, intptr_t p2);
public:
    PlatformRenderer(weak<Renderer> renderer);
    ~PlatformRenderer();

    HWND            createWindowImplementation(const WindowCreationFlags* flags) const;
    void            destroyWindowImplementation(HWND hWnd);

    void            postMessage(UINT msg, WPARAM wParam, LPARAM lParam) const;
    void            handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    const istring&  getWindowClassName() const;
};


}}}

/*****************************************************************************/
#endif
