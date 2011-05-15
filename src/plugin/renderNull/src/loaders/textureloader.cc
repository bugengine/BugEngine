/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/textureloader.script.hh>
#include    <graphics/objects/texture.script.hh>

namespace BugEngine { namespace Graphics { namespace NullRenderer
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
