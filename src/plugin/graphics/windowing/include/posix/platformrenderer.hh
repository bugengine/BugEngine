/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WINDOWING_POSIX_RENDERER_HH_
#define BE_WINDOWING_POSIX_RENDERER_HH_
/**************************************************************************************************/
#include    <windowing/renderer.hh>
#include    <GL/glx.h>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Windowing
{

struct PlatformData
{
    ::Display*      display;
    ::GLXFBConfig   fbConfig;
    ::XVisualInfo*  visual;
    PlatformData(::Display* display);
};

class Renderer::PlatformRenderer : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
    PlatformData    m_platformData;
    ::Atom          m_windowProperty;
private:
    static int xError(::Display* display, XErrorEvent* event);
public:
    PlatformRenderer();
    ~PlatformRenderer();
    ::Window createWindow(i16 x, i16 y, u16 w, u16 h);
};

}}


/**************************************************************************************************/
#endif

