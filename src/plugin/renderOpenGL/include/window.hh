/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_WINDOW_HH_
#define BE_OPENGL_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class Window : public Windowing::Window
{
    friend class Renderer;
private:
    weak<Renderer>  m_owner;
public:
    Window(weak<Renderer> renderer, WindowFlags flags, ref<const Scene> scene);
    ~Window();

    void setCurrent() override;

    void begin();
    void end();

    void close() override;

    bool closed() const override;
};

}}}

/*****************************************************************************/
#endif
