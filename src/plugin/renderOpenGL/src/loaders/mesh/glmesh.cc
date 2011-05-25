/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <graphics/objects/mesh.script.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLMesh::GLMesh(weak<const Resource> resource, weak<const Renderer> renderer)
    :   IGPUResource(resource, renderer)
{
}

GLMesh::~GLMesh()
{
}

void GLMesh::load(weak<const Resource> resource)
{
}

void GLMesh::unload()
{
}

}}}
