/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <graphics/objects/mesh.script.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLMesh::GLMesh(weak<const Resource> resource)
    :   IGPUResource(resource)
{
}

GLMesh::~GLMesh()
{
}

void GLMesh::load(weak<IRenderer> renderer)
{
}

void GLMesh::unload(weak<IRenderer> renderer)
{
}

}}}
