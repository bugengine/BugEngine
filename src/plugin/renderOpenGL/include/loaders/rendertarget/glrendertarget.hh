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
    void setCurrent();
    void clearCurrent();
    bool closed() const override;
public:
    GLRenderTarget(weak<const Resource> resource, weak<const Renderer> renderer);
    ~GLRenderTarget();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    void present();
};

}}}

/*****************************************************************************/
#endif
