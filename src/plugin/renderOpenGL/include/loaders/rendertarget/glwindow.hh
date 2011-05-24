/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_RENDERTARGET_GLWINDOW_HH_
#define BE_OPENGL_LOADERS_RENDERTARGET_GLWINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class GLWindow : public Windowing::Window
{
    friend class Renderer;
private:
    class Context;
    scoped<Context> m_context;
private:
    void setCurrent();
    void clearCurrent();
    void present();
private:
    void load() override;
    void unload() override;
public:
    virtual void    begin(ClearMode clear) override;
    virtual void    end(PresentMode present) override;
public:
    GLWindow(weak<const RenderTarget> rendertarget, weak<const Renderer> renderer);
    ~GLWindow();
};

}}}

/*****************************************************************************/
#endif
