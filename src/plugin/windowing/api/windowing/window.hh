/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_WINDOW_HH_
#define BE_WINDOWING_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/irendertarget.hh>
#include    <graphics/renderer/irenderer.hh>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Renderer;

class Window : public IRenderTarget
{
private:
    class PlatformWindow;
    scoped<PlatformWindow>  m_window;
protected:
    void* getWindowHandle() const;
    bool closed() const;
public:
    Window(weak<const RenderTarget> resource, weak<const Renderer> renderer);
    ~Window();

    void load();
    void unload();

    void close();
    uint2 getDimensions() const;
};

}}}

/*****************************************************************************/
#endif
