/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SCENEGRAPH_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SCENEGRAPH_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

class IScene;
class RenderTarget;
class SceneGraphLoader;

class be_api(GRAPHICS) RenderNode : public Resource
{
protected:
    RenderNode();
    ~RenderNode();
};

be_tag(ResourceLoaders())
class be_api(GRAPHICS) RenderScene : public RenderNode
{
    friend class SceneGraphLoader;
private:
    ref<IScene>         m_scene;
    ref<RenderTarget>   m_rendertarget;
published:
    RenderScene(ref<IScene> scene, ref<RenderTarget> rendertarget);
    ~RenderScene();
};

be_tag(ResourceLoaders())
class be_api(GRAPHICS) RenderSequence : public RenderNode
{
    friend class SceneGraphLoader;
private:
    minitl::vector< ref<const RenderNode> > m_nodes;
published:
    RenderSequence(const minitl::vector< ref<const RenderNode> >& nodes);
    ~RenderSequence();
};

}}

/*****************************************************************************/
#endif
