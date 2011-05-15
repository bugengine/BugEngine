/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/shaderloader.script.hh>
#include    <graphics/objects/shader.script.hh>

namespace BugEngine { namespace Graphics { namespace NullRenderer
{

ShaderLoader::ShaderLoader()
{
    attach<Shader>();
}

ShaderLoader::~ShaderLoader()
{
    detach<Shader>();
}

void* ShaderLoader::load(weak<const Resource> source) const
{
    return 0;
}

void  ShaderLoader::unload(const void* resource) const
{
}

}}}
