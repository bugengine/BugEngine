/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/mesh/meshloader.script.hh>
#include    <graphics/objects/mesh.script.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

MeshLoader::MeshLoader()
{
    attach<Mesh>();
}

MeshLoader::~MeshLoader()
{
    detach<Mesh>();
}

void* MeshLoader::load(weak<const Resource> source) const
{
    return 0;
}

void  MeshLoader::unload(const void* resource) const
{
}

}}}
