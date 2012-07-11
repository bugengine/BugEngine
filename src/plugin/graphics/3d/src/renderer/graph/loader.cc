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

ResourceHandle SceneGraphLoader::load(weak<const Resource> source)
{
    ResourceHandle handle;
    handle.handle = be_checked_cast<const RenderNode>(source)->createNode(this, m_renderer);
    return handle;
}

void SceneGraphLoader::unload(const ResourceHandle& /*resource*/)
{
}

}
