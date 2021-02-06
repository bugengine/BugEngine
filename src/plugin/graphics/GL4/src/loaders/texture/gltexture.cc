/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.3d/texture/texture.script.hh>
#include <bugengine/plugin.graphics.GL4/glrenderer.hh>
#include <loaders/texture/gltexture.hh>

namespace BugEngine { namespace OpenGL {

GLTexture::GLTexture(weak< const Resource::Description > textureDescription,
                     weak< GLRenderer >                  renderer)
    : IGPUResource(textureDescription, renderer)
{
}

GLTexture::~GLTexture()
{
}

void GLTexture::load(weak< const Resource::Description > /*textureDescription*/)
{
}

void GLTexture::unload()
{
}

}}  // namespace BugEngine::OpenGL
