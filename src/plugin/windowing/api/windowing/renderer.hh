/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_RENDERER_HH_
#define BE_WINDOWING_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>
#include    <core/threads/thread.hh>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window;
class RendererData;
struct WindowCreationFlags;

class Renderer : public IRenderer
{
    friend class Window;
private:
    istring         m_windowClassName;
    WNDCLASSEX      m_wndClassEx;
    Thread          m_windowManagementThread;
private:
    static intptr_t updateWindows(intptr_t p1, intptr_t p2);
protected:
    void            flush() override;
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
