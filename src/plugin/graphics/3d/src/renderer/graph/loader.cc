/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <graph/loader.hh>

#include    <3d/renderer/irenderer.hh>
#include    <graph/scenenode.hh>
#include    <graph/multinode.hh>
#include    <3d/scene/scenegraph.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/renderer/irendertarget.hh>


namespace BugEngine
{

SceneGraphLoader::SceneGraphLoader(weak<const IRenderer> renderer)
    :   m_renderer(renderer)
{
}

SceneGraphLoader::~SceneGraphLoader()
{
}

void SceneGraphLoader::load(weak<const Resource::Description> description, Resource::Resource& resource)
{
    resource.setRefHandle(be_checked_cast<const RenderNode>(description)->createNode(this, m_renderer));
}

void SceneGraphLoader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}
