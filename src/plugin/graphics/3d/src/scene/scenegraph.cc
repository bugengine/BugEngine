/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/scene/scenegraph.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/renderer/irenderer.hh>
#include    <3d/renderer/irendertarget.hh>
#include    <graph/inode.hh>
#include    <graph/loader.hh>
#include    <graph/scenenode.hh>
#include    <graph/multinode.hh>
#include    <world/world.script.hh>

namespace BugEngine
{

RenderNode::RenderNode()
{
}

RenderNode::~RenderNode()
{
}

RenderScene::RenderScene(ref<RenderTarget> rendertarget, ref<const World> world)
    :   RenderNode()
    ,   m_renderTarget(rendertarget)
    ,   m_world(world)
{
}

RenderScene::~RenderScene()
{
}

ref<INode> RenderScene::createNode(weak<const SceneGraphLoader> /*loader*/, weak<const IRenderer> renderer) const
{
    weak<IGPUResource> renderTarget = renderer->getRenderSurface(m_renderTarget);
    if (!renderTarget)
    {
        renderTarget = renderer->getRenderWindow(m_renderTarget);
    }
    be_assert_recover(renderTarget, "can't create scene node: render target has not been created yet", return ref<INode>());
    return ref<SceneNode>::create(gameArena(), be_checked_cast<IRenderTarget>(renderTarget), m_world);
}

RenderSequence::RenderSequence(const minitl::vector< ref<const RenderNode> >& nodes)
    :   RenderNode()
    ,   m_nodes(nodes)
{
}

RenderSequence::~RenderSequence()
{
}

ref<INode> RenderSequence::createNode(weak<const SceneGraphLoader> loader, weak<const IRenderer> /*renderer*/) const
{
    minitl::vector< weak<INode> > nodes(tempArena());
    for (minitl::vector< ref<const RenderNode> >::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
    {
        weak<minitl::pointer> childhandle = (*it)->getResourceHandle(loader).handle;
        be_assert(childhandle, "dependent node was not loaded properly");
        if (childhandle) nodes.push_back(be_checked_cast<INode>(childhandle));
    }

    return ref<MultiNode>::create(gameArena(), nodes);
}

}
