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
public:
    GLRenderTarget(weak<const Resource> resource, weak<const Renderer> renderer);
    ~GLRenderTarget();

    void setCurrent();
    void clearCurrent();

    virtual void load() override;
    virtual void unload() override;

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    bool closed() const override { return false; }

    void present();
};

}}}

/*****************************************************************************/
#endif
