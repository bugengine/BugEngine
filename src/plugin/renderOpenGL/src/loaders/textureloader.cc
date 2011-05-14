/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/textureloader.script.hh>
#include    <graphics/objects/texture.script.hh>

#ifdef BE_PLATFORM_MACOS
# include   <OpenGL/OpenGL.h>
# include   <OpenGL/gl.h>
#else
# include   <GL/gl.h>
# include   <GL/gl3.h>
#endif

namespace BugEngine { namespace Graphics { namespace OpenGL
{

TextureLoader::TextureLoader()
{
    attach<Texture>();
}

TextureLoader::~TextureLoader()
{
    detach<Texture>();
}

void* TextureLoader::load(weak<const Resource> source) const
{
    return 0;
}

void  TextureLoader::unload(const void* resource) const
{
}

}}}
