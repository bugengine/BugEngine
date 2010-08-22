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
    #ifdef BE_PLATFORM_WIN32
        HGLRC       m_glContext;
    #else
        GLXContext  m_glContext;
    #endif
private:
    inline void setCurrent();
    inline void clearCurrent();
public:
    Window(weak<Renderer> renderer, WindowFlags flags);
    ~Window();

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    void close() override;
    bool closed() const override;
};

}}}

/*****************************************************************************/
#endif
