/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/scene/scenegraph.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/renderer/irendertarget.hh>

namespace BugEngine { namespace Graphics
{

RenderNode::RenderNode()
{
}

RenderNode::~RenderNode()
{
}

RenderScene::RenderScene(ref<RenderTarget> rendertarget)
    :   RenderNode()
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
