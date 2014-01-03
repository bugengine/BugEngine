/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
#include    <loaders/texture/gltexture.hh>
#include    <3d/texture/texture.script.hh>


namespace BugEngine { namespace OpenGL
{

GLTexture::GLTexture(weak<const Resource::Description> textureDescription, weak<GLRenderer> renderer)
    :   IGPUResource(textureDescription, renderer)
{
}

GLTexture::~GLTexture()
{
}

void GLTexture::load(weak<const Resource::Description> /*textureDescription*/)
{
}

void GLTexture::unload()
{
}

}}
