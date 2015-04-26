/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASK_TASK_HH_
#define BE_SCHEDULER_TASK_TASK_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/task/itask.hh>
#include    <scheduler/private/taskitem.hh>

namespace BugEngine { namespace Task
{

template< typename Body >
class Task : public ITask
{
    template< class B, class R >
    friend class TaskItem;
    BE_NOCOPY(Task);
public:
    Body            body;
private:
    mutable i_u32   m_taskCount;
    mutable i_u32   m_taskCompleted;
public:
    Task(istring name, color32 color, const Body& body,
         Scheduler::Priority priority = Scheduler::Default,
         Scheduler::Affinity affinity = Scheduler::DontCare);
    virtual void schedule(weak<Scheduler> sc) override;
};

}}

#include    <scheduler/task/task.inl>

/**************************************************************************************************/
#endif
