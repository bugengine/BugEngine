/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/scenegraph.script.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <graphics/scene/iscene.script.hh>
#include    <graphics/renderer/irendertarget.hh>

namespace BugEngine { namespace Graphics
{

RenderNode::RenderNode()
{
}

RenderNode::~RenderNode()
{
}

RenderScene::RenderScene(ref<IScene> scene, ref<RenderTarget> rendertarget)
    :   RenderNode()
    ,   m_scene(scene)
    ,   m_rendertarget(rendertarget)
{
}

RenderScene::~RenderScene()
{
}

RenderSequence::RenderSequence(const minitl::vector< ref<const RenderNode> >& nodes)
    :   RenderNode()
    ,   m_nodes(nodes)
{
}

RenderSequence::~RenderSequence()
{
}

}}
