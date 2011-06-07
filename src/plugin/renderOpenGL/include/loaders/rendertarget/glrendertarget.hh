/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_RENDERTARGET_GLRENDERTARGET_HH_
#define BE_OPENGL_LOADERS_RENDERTARGET_GLRENDERTARGET_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class GLRenderTarget : public IRenderTarget
{
private:
    void setCurrent() const;
    void clearCurrent() const;
public:
    GLRenderTarget(weak<const Resource> resource, weak<Renderer> renderer);
    ~GLRenderTarget();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}}

/*****************************************************************************/
#endif
