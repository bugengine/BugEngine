/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.3d/mesh/mesh.script.hh>
#include <bugengine/plugin.graphics.GL4/glrenderer.hh>
#include <loaders/mesh/glmesh.hh>

namespace BugEngine { namespace OpenGL {

GLMesh::GLMesh(weak< const Resource::Description > meshDescription, weak< GLRenderer > renderer)
    : IGPUResource(meshDescription, renderer)
{
}

GLMesh::~GLMesh()
{
}

void GLMesh::load(weak< const Resource::Description > /*meshDescription*/)
{
}

void GLMesh::unload()
{
}

}}  // namespace BugEngine::OpenGL
