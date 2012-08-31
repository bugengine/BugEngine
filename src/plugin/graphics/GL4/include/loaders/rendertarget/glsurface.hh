/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_RENDERTARGET_GLSURFACE_HH_
#define BE_GL4_LOADERS_RENDERTARGET_GLSURFACE_HH_
/*****************************************************************************/
#include    <3d/renderer/igpuresource.hh>

namespace BugEngine { namespace OpenGL
{

class GLRenderer;

class GLSurface : public IRenderTarget
{
private:
    void setCurrent() const;
    void clearCurrent() const;
public:
    GLSurface(weak<const RenderSurfaceDescription> surfaceDescription, weak<GLRenderer> renderer);
    ~GLSurface();

    virtual void load(weak<const Resource::Description> surfaceDescription) override;
    virtual void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}

/*****************************************************************************/
#endif
