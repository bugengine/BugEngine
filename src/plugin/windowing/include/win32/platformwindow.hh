/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_WIN32_PLATFORMWINDOW_HH_
#define BE_WINDOWING_WIN32_PLATFORMWINDOW_HH_
/*****************************************************************************/
#include    <windows.h>


namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window::PlatformWindow : public minitl::refcountable
{
    friend class Window;
private:
    weak<const Renderer>    m_renderer;
    HWND                    m_window;
public:
    PlatformWindow(weak<const Renderer> renderer, weak<Window> window);
    ~PlatformWindow();
};

}}}

/*****************************************************************************/
#endif
