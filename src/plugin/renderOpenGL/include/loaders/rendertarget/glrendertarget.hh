/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_RENDERTARGET_GLRENDERTARGET_HH_
#define BE_OPENGL_LOADERS_RENDERTARGET_GLRENDERTARGET_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class GLRenderTarget : public IGPUResource
{
public:
    GLRenderTarget(weak<const Resource> resource);
    ~GLRenderTarget();

    virtual void load(weak<IRenderer> renderer) override;
    virtual void unload(weak<IRenderer> renderer) override;
};

}}}

/*****************************************************************************/
#endif
