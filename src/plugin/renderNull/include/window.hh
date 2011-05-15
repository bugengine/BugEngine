/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_WINDOW_HH_
#define BE_RENDERNULL_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/irendertarget.hh>
#include    <graphics/renderer/irenderer.hh>


namespace BugEngine { namespace Graphics { namespace NullRenderer
{

class Renderer;

class Window : public IRenderTarget
{
private:
    WindowFlags m_flags;
    bool        m_closed;
public:
    Window(weak<Renderer> renderer, WindowFlags flags);
    ~Window();

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    void present();
    bool closed() const override;
    void close() override;
    uint2 getDimensions() const override { return m_flags.size; }
};

}}}

/*****************************************************************************/
#endif
