/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_MESH_GLMESH_HH_
#define BE_OPENGL_LOADERS_MESH_GLMESH_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class GLMesh : public IGPUResource
{
public:
    GLMesh(weak<const Resource> resource, weak<const Renderer> renderer);
    ~GLMesh();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;
};

}}}

/*****************************************************************************/
#endif
