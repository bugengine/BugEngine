/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_ISCENE_HH_
#define BE_GRAPHICS_SCENE_ISCENE_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <system/scheduler/task/itask.hh>

namespace BugEngine { namespace Graphics
{

class RenderTarget;

class IScene : public minitl::refcountable
{
protected:
    ref<ITask>          m_updateTask;
public:
    IScene();
    virtual ~IScene();

    virtual weak<ITask> updateTask() const;
    virtual ref<ITask> createRenderTask(weak<const RenderTarget> renderTarget) const = 0;
};

}}

/*****************************************************************************/
#endif
