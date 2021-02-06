/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_TEXTURE_GLTEXTURE_HH_
#define BE_GL4_LOADERS_TEXTURE_GLTEXTURE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>

namespace BugEngine { namespace OpenGL {

class GLRenderer;

class GLTexture : public IGPUResource
{
public:
    GLTexture(weak< const Resource::Description > textureDescription, weak< GLRenderer > renderer);
    ~GLTexture();

    virtual void load(weak< const Resource::Description > textureDescription) override;
    virtual void unload() override;
};

}}  // namespace BugEngine::OpenGL

/**************************************************************************************************/
#endif
