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
    void present();
public:
    Window(weak<Renderer> renderer);
    ~Window();
};

}}}

/*****************************************************************************/
#endif
