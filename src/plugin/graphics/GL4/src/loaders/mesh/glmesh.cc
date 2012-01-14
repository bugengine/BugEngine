/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <3d/mesh/mesh.script.hh>


namespace BugEngine { namespace OpenGL
{

GLMesh::GLMesh(weak<const Resource> resource, weak<GLRenderer> renderer)
    :   IGPUResource(resource, renderer)
{
}

GLMesh::~GLMesh()
{
}

void GLMesh::load(weak<const Resource> /*resource*/)
{
}

void GLMesh::unload()
{
}

}}
