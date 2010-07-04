/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE_HH_
#define BE_GRAPHICS_SCENE_SCENE_HH_
/*****************************************************************************/
#include    <graphics/scene/iscene.hh>
#include    <system/scheduler/task/itask.hh>

namespace BugEngine { namespace Graphics
{

template< class SpacePartitioning >
class Scene : public IScene
{
private:
    SpacePartitioning   m_scenePartition;
public:
    typedef typename SpacePartitioning::Node Node;
public:
    Node root();
    virtual ref<ITask> createRenderTask(weak<const RenderTarget> renderTarget) const override;
};

}}

#include    <graphics/scene/scene.inl>

/*****************************************************************************/
#endif
