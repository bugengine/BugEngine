/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/texture/gltexture.hh>
#include    <graphics/objects/texture.script.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLTexture::GLTexture(weak<const Resource> resource, weak<const Renderer> renderer)
    :   IGPUResource(resource, renderer)
{
}

GLTexture::~GLTexture()
{
}

void GLTexture::load()
{
}

void GLTexture::unload()
{
}
}}}
