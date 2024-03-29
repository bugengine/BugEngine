/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_WIN32_PLATFORMRENDERER_HH_
#define BE_WINDOWING_WIN32_PLATFORMRENDERER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.windowing/stdafx.h>
#include <bugengine/core/threads/thread.hh>
#include <windows.h>

namespace BugEngine { namespace Windowing {

struct WindowCreationFlags
{
    const char* title;
    int         x;
    int         y;
    RECT        size;
    DWORD       flags;
    bool        fullscreen;
};

class Renderer::PlatformRenderer : public minitl::refcountable
{
    BE_NOCOPY(PlatformRenderer);

private:
    weak< Renderer > m_renderer;
    istring          m_windowClassName;
    WNDCLASSEX       m_wndClassEx;

public:
    PlatformRenderer(weak< Renderer > renderer);
    ~PlatformRenderer();

    HWND           createWindowImplementation(const WindowCreationFlags& flags) const;
    void           destroyWindowImplementation(HWND hWnd);
    const istring& getWindowClassName() const;
};

}}  // namespace BugEngine::Windowing

/**************************************************************************************************/
#endif
