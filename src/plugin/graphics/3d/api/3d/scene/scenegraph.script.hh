/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_SCENE_SCENEGRAPH_SCRIPT_HH_
#define BE_3D_SCENE_SCENEGRAPH_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine
{

class World;
class RenderTarget;
class SceneGraphLoader;
class INode;
class IRenderer;

class be_api(_3D) RenderNode : public Resource
{
    friend class SceneGraphLoader;
    BE_NOCOPY(RenderNode);
protected:
    RenderNode();
    ~RenderNode();
protected:
    virtual ref<INode> createNode(weak<const SceneGraphLoader> loader, weak<const IRenderer> renderer) const = 0;
};

class be_api(_3D) RenderScene : public RenderNode
{
    BE_NOCOPY(RenderScene);
private:
    ref<RenderTarget>   m_renderTarget;
    ref<const World>    m_world;
published:
    RenderScene(ref<RenderTarget> rendertarget, ref<const World> world);
    ~RenderScene();
private:
    virtual ref<INode> createNode(weak<const SceneGraphLoader> loader, weak<const IRenderer> renderer) const override;
};

class be_api(_3D) RenderSequence : public RenderNode
{
    BE_NOCOPY(RenderSequence);
private:
    minitl::vector< ref<const RenderNode> > m_nodes;
published:
    RenderSequence(const minitl::vector< ref<const RenderNode> >& nodes);
    ~RenderSequence();
private:
    virtual ref<INode> createNode(weak<const SceneGraphLoader> loader, weak<const IRenderer> renderer) const override;
};

}

/*****************************************************************************/
#endif
