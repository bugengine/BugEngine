/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE_INL_
#define BE_GRAPHICS_SCENE_SCENE_INL_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/range/onestep.hh>
#include    <graphics/renderer/renderbackend.hh>
#include    <graphics/renderer/rendertarget.hh>

namespace BugEngine { namespace Graphics
{

template< class SpacePartitioning >
typename Scene<SpacePartitioning>::Node Scene<SpacePartitioning>::root()
{
    return Node();
}

template< class SpacePartitioning >
ref<ITask> Scene<SpacePartitioning>::createRenderTask(weak<const RenderTarget> renderTarget) const
{
    struct RenderJob
    {
    private:
        weak< const Scene<SpacePartitioning> >  m_scene;
        weak<const RenderTarget>                m_renderTarget;
    public:
        RenderJob(weak< const Scene<SpacePartitioning> > scene, weak<const RenderTarget> renderTarget)
            :   m_scene(scene)
            ,   m_renderTarget(renderTarget)
        {
        }

        typedef SpacePartitioning::Node Range;

        Range prepare() { return Range(); }
        void operator()(const Range& /*r*/) const
        {
        }
        void operator()(Range& /*myRange*/, RenderJob& /*with*/, Range& /*withRange*/)
        {
        }
    };

    ref<ITask> result = ref< Task<RenderJob> >::create("render", color32(255,0,0),  RenderJob(this, renderTarget));
    result->addCallback(renderTarget->flushTask()->startCallback());
    renderTarget->flushTask()->addCallback(result->startCallback(), ITask::ICallback::CallbackStatus_Completed);
    return result;
}

}}

/*****************************************************************************/
#endif
