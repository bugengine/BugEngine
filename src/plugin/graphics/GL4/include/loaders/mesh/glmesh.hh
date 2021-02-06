/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_MESH_GLMESH_HH_
#define BE_GL4_LOADERS_MESH_GLMESH_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>

namespace BugEngine { namespace OpenGL {

class GLRenderer;

class GLMesh : public IGPUResource
{
public:
    GLMesh(weak< const Resource::Description > meshDescription, weak< GLRenderer > renderer);
    ~GLMesh();

    virtual void load(weak< const Resource::Description > meshDescription) override;
    virtual void unload() override;
};

}}  // namespace BugEngine::OpenGL

/**************************************************************************************************/
#endif
