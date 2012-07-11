/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
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
