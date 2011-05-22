/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_WINDOW_HH_
#define BE_WINDOWING_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/objects/rendertarget.script.hh>
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
    bool closed() const override;
public:
    Window(weak<Renderer> renderer, WindowFlags flags);
    ~Window();

    void close() override;
    uint2 getDimensions() const override;
};

}}}

/*****************************************************************************/
#endif
