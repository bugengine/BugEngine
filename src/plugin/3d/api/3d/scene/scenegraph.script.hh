/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_SCENE_SCENEGRAPH_SCRIPT_HH_
#define BE_3D_SCENE_SCENEGRAPH_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine { namespace Graphics
{

class RenderTarget;
class SceneGraphLoader;

class be_api(_3D) RenderNode : public Resource
{
    BE_NOCOPY(RenderNode);
protected:
    RenderNode();
    ~RenderNode();
};

be_tag(ResourceLoaders())
class be_api(_3D) RenderScene : public RenderNode
{
    friend class SceneGraphLoader;
    BE_NOCOPY(RenderScene);
private:
    ref<RenderTarget>   m_rendertarget;
published:
    RenderScene(ref<RenderTarget> rendertarget);
    ~RenderScene();
};

be_tag(ResourceLoaders())
class be_api(_3D) RenderSequence : public RenderNode
{
    friend class SceneGraphLoader;
    BE_NOCOPY(RenderSequence);
private:
    minitl::vector< ref<const RenderNode> > m_nodes;
published:
    RenderSequence(const minitl::vector< ref<const RenderNode> >& nodes);
    ~RenderSequence();
};

}}

/*****************************************************************************/
#endif
