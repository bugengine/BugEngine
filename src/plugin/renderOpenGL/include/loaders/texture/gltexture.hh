/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_TEXTURE_GLTEXTURE_HH_
#define BE_OPENGL_LOADERS_TEXTURE_GLTEXTURE_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class GLTexture : public IGPUResource
{
public:
    GLTexture(weak<const Resource> resource);
    ~GLTexture();

    virtual void load(weak<IRenderer> renderer) override;
    virtual void unload(weak<IRenderer> renderer) override;
};

}}}

/*****************************************************************************/
#endif
