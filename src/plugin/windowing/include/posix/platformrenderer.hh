/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_POSIX_RENDERER_HH_
#define BE_WINDOWING_POSIX_RENDERER_HH_
/*****************************************************************************/
#include    <windowing/renderer.hh>
#include    <GL/glx.h>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Renderer::PlatformRenderer : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
    ::Display*      m_display;
    int             m_screen;
    ::Window        m_rootWindow;
    GLXFBConfig     m_fbConfig;
    ::XVisualInfo*  m_visual;
    ::Atom          m_windowProperty;
private:
    static int xError(::Display* display, XErrorEvent* event);
public:
    PlatformRenderer();
    ~PlatformRenderer();
    ::Window createWindow(const WindowFlags& flags);
};

}}}


/*****************************************************************************/
#endif

