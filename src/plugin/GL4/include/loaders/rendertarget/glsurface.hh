/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_RENDERTARGET_GLSURFACE_HH_
#define BE_OPENGL_LOADERS_RENDERTARGET_GLSURFACE_HH_
/*****************************************************************************/
#include    <3d/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class GLRenderer;

class GLSurface : public IRenderTarget
{
private:
    void setCurrent() const;
    void clearCurrent() const;
public:
    GLSurface(weak<const RenderSurface> resource, weak<GLRenderer> renderer);
    ~GLSurface();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}}

/*****************************************************************************/
#endif
