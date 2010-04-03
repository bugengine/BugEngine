/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE_HH_
#define BE_GRAPHICS_SCENE_SCENE_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <system/scheduler/task/itask.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Scene : public minitl::refcountable
{
protected:
    ref<ITask>  m_updateTask;
public:
    Scene();
    virtual ~Scene();

    virtual weak<ITask> updateTask() const;
};

}}


/*****************************************************************************/
#endif
