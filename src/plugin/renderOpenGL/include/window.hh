/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_WINDOW_HH_
#define BE_OPENGL_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class Window : public Windowing::Window
{
    friend class Renderer;
private:
    class Context;
    scoped<Context> m_context;
private:
    void setCurrent();
    void clearCurrent();
public:
    Window(weak<Renderer> renderer, WindowFlags flags);
    ~Window();

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    void present();
};

}}}

/*****************************************************************************/
#endif
