/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_TEXTURE_GLTEXTURE_HH_
#define BE_GL4_LOADERS_TEXTURE_GLTEXTURE_HH_
/*****************************************************************************/
#include    <3d/renderer/igpuresource.hh>

namespace BugEngine { namespace OpenGL
{

class GLRenderer;

class GLTexture : public IGPUResource
{
public:
    GLTexture(weak<const Resource> resource, weak<GLRenderer> renderer);
    ~GLTexture();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;
};


}}

/*****************************************************************************/
#endif
