/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_RENDERTARGET_GLSURFACE_HH_
#define BE_GL4_LOADERS_RENDERTARGET_GLSURFACE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>

namespace BugEngine { namespace OpenGL {

class GLRenderer;

class GLSurface : public IRenderTarget
{
private:
    void setCurrent() const;
    void clearCurrent() const;

public:
    GLSurface(weak< const RenderSurfaceDescription > surfaceDescription,
              weak< GLRenderer >                     renderer);
    ~GLSurface();

    virtual void load(weak< const Resource::Description > surfaceDescription) override;
    virtual void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}  // namespace BugEngine::OpenGL

/**************************************************************************************************/
#endif
