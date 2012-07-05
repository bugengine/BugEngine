/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
#include    <loaders/texture/gltexture.hh>
#include    <3d/texture/texture.script.hh>


namespace BugEngine { namespace OpenGL
{

GLTexture::GLTexture(weak<const Resource> resource, weak<GLRenderer> renderer)
    :   IGPUResource(resource, renderer)
{
}

GLTexture::~GLTexture()
{
}

void GLTexture::load(weak<const Resource> /*resource*/)
{
}

void GLTexture::unload()
{
}

}}
