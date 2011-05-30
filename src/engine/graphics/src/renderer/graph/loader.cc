/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graph/loader.hh>

#include    <graphics/renderer/irenderer.hh>
#include    <graph/scenenode.hh>
#include    <graph/multinode.hh>
#include    <graphics/scene/iscene.script.hh>
#include    <graphics/objects/scenegraph.script.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <graphics/renderer/irendertarget.hh>


namespace BugEngine { namespace Graphics
{

SceneGraphLoader::SceneGraphLoader(weak<const IRenderer> renderer)
    :   m_renderer(renderer)
{
    ResourceLoaders::attach<RenderScene, SceneGraphLoader>(this, &SceneGraphLoader::load, &SceneGraphLoader::unload);
    ResourceLoaders::attach<RenderSequence, SceneGraphLoader>(this, &SceneGraphLoader::load, &SceneGraphLoader::unload);
}

SceneGraphLoader::~SceneGraphLoader()
{
    ResourceLoaders::detach<RenderSequence, SceneGraphLoader>(this);
    ResourceLoaders::detach<RenderScene, SceneGraphLoader>(this);
}

ResourceHandle SceneGraphLoader::load(weak<const RenderScene> source)
{
    ResourceHandle handle;
    weak<minitl::pointer> renderhandle = source->m_rendertarget->getResource(m_renderer).handle;
    be_assert_recover(renderhandle, "can't create scene node: render target has not been created yet", return handle);
    handle.handle = ref<SceneNode>::create(gameArena(), source->m_scene, be_checked_cast<IRenderTarget>(renderhandle));
    return handle;
}

ResourceHandle SceneGraphLoader::load(weak<const RenderSequence> source)
{
    ResourceHandle handle;
    minitl::vector< weak<INode> > nodes (tempArena());
    for (minitl::vector< ref<const RenderNode> >::const_iterator it = source->m_nodes.begin(); it != source->m_nodes.end(); ++it)
    {
        weak<minitl::pointer> childhandle = (*it)->getResource(this).handle;
        be_assert(childhandle, "dependent node was not loaded properly");
        if (childhandle) nodes.push_back(be_checked_cast<INode>(childhandle));
    }
    handle.handle = ref<MultiNode>::create(gameArena(), nodes);
    return handle;
}

void SceneGraphLoader::unload(const ResourceHandle& /*resource*/)
{
}

}}
