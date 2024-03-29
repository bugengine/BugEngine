/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_WIN32_PLATFORMWINDOW_HH_
#define BE_WINDOWING_WIN32_PLATFORMWINDOW_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.windowing/stdafx.h>
#include <windows.h>

namespace BugEngine { namespace Windowing {
class Window;

class Window::PlatformWindow : public minitl::refcountable
{
    friend class Window;
    BE_NOCOPY(PlatformWindow);

private:
    weak< const Renderer > m_renderer;
    HWND                   m_window;

public:
    PlatformWindow(weak< const Renderer > renderer, weak< Window > window);
    ~PlatformWindow();
};

}}  // namespace BugEngine::Windowing

/**************************************************************************************************/
#endif
