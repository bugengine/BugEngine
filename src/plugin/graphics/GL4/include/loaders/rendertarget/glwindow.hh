/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_RENDERTARGET_GLWINDOW_HH_
#define BE_GL4_LOADERS_RENDERTARGET_GLWINDOW_HH_
/*****************************************************************************/
#include    <3d/renderer/irenderer.hh>


namespace BugEngine { namespace OpenGL
{

class GLRenderer;

class GLWindow : public Windowing::Window
{
    friend class GLRenderer;
    BE_NOCOPY(GLWindow);
private:
    class Context;
    scoped<Context> m_context;
private:
    void setCurrent() const;
    void clearCurrent() const;
    void present() const;
private:
    void load(weak<const Resource::Description> windowDescription) override;
    void unload() override;
public:
    virtual void    begin(ClearMode clear) const override;
    virtual void    end(PresentMode present) const override;
public:
    GLWindow(weak<const RenderWindowDescription> windowDescription, weak<const GLRenderer> renderer);
    ~GLWindow();
};

}}

/*****************************************************************************/
#endif
