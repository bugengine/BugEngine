/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASK_TASK_HH_
#define BE_SYSTEM_SCHEDULER_TASK_TASK_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>
#include    <system/scheduler/private/taskitem.hh>

namespace BugEngine
{

template< typename Body >
class Task : public ITask
{
    template< class B, class R >
    friend class ScheduledTasks::TaskItem;
    BE_NOCOPY(Task);
private:
    mutable Body    m_body;
    mutable i_u32   m_taskCount;
    mutable i_u32   m_taskCompleted;
public:
    Task(istring name, color32 color, const Body& body, Scheduler::Priority priority = Scheduler::Default, Scheduler::Affinity affinity = Scheduler::DontCare);
    virtual void run(weak<Scheduler> sc) const override;
};

}

#include    <system/scheduler/task/task.inl>

/*****************************************************************************/
#endif
