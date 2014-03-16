/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_MESH_GLMESH_HH_
#define BE_GL4_LOADERS_MESH_GLMESH_HH_
/**************************************************************************************************/
#include    <GL4/stdafx.h>
#include    <3d/renderer/igpuresource.hh>

namespace BugEngine { namespace OpenGL
{

class GLRenderer;

class GLMesh : public IGPUResource
{
public:
    GLMesh(weak<const Resource::Description> meshDescription, weak<GLRenderer> renderer);
    ~GLMesh();

    virtual void load(weak<const Resource::Description> meshDescription) override;
    virtual void unload() override;
};

}}

/**************************************************************************************************/
#endif
