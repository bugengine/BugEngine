/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_SCENE_SCENEGRAPH_SCRIPT_HH_
#define BE_3D_SCENE_SCENEGRAPH_SCRIPT_HH_
/*****************************************************************************/
#include    <resource/description.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <world/world.script.hh>

namespace BugEngine
{

namespace World
{
class World;
}

class RenderTargetDescription;
class SceneGraphLoader;
class INode;
class IRenderer;

class be_api(_3D) RenderNode : public Resource::Description
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
    ref<RenderTargetDescription>    m_renderTarget;
    ref<const World::World>         m_world;
published:
    RenderScene(ref<RenderTargetDescription> renderTarget, ref<const World::World> world);
    ~RenderScene();
private:
    virtual ref<INode> createNode(weak<const SceneGraphLoader> loader, weak<const IRenderer> renderer) const override;
};

class be_api(_3D) RenderSequence : public RenderNode
{
    BE_NOCOPY(RenderSequence);
private:
    minitl::array< ref<const RenderNode> > m_nodes;
published:
    RenderSequence(const minitl::array< ref<const RenderNode> >& nodes);
    ~RenderSequence();
private:
    virtual ref<INode> createNode(weak<const SceneGraphLoader> loader, weak<const IRenderer> renderer) const override;
};

}

/*****************************************************************************/
#endif
