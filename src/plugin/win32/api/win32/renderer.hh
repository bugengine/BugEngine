/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WIN32_RENDERER_HH_
#define BE_WIN32_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>
#include    <core/threads/thread.hh>

namespace BugEngine { namespace Graphics { namespace Win32
{

class Window;

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

class Renderer : public RenderBackend
{
    friend class Window;
private:
    istring         m_windowClassName;
    WNDCLASSEX      m_wndClassEx;
    Thread          m_windowManagementThread;
private:
    static intptr_t updateWindows(intptr_t p1, intptr_t p2);
private:
    HWND            createWindowImplementation(const WindowCreationFlags* flags) const;
    void            destroyWindowImplementation(HWND hWnd);
protected:
    virtual UINT    messageCount() const;
    virtual void    handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void            postMessage(UINT msg, WPARAM wParam, LPARAM lParam) const;
public:
    Renderer();
    ~Renderer();

    uint2           getScreenSize() override;
    const istring&  getWindowClassName() const;
};

}}}

/*****************************************************************************/
#endif
