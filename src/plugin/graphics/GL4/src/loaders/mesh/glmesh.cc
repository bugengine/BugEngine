/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <3d/mesh/mesh.script.hh>


namespace BugEngine { namespace OpenGL
{

GLMesh::GLMesh(weak<const Resource::Description> meshDescription, weak<GLRenderer> renderer)
    :   IGPUResource(meshDescription, renderer)
{
}

GLMesh::~GLMesh()
{
}

void GLMesh::load(weak<const Resource::Description> /*meshDescription*/)
{
}

void GLMesh::unload()
{
}

}}
