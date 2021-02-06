/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASK_TASK_HH_
#define BE_SCHEDULER_TASK_TASK_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/private/taskitem.hh>
#include <bugengine/scheduler/task/itask.hh>

namespace BugEngine { namespace Task {

template < typename Body >
class Task : public ITask
{
    template < class B, class R >
    friend class TaskItem;
    BE_NOCOPY(Task);

public:
    Body body;

private:
    i_u32 m_taskCount;
    i_u32 m_taskCompleted;

public:
    Task(istring name, color32 color, const Body& body,
         Scheduler::Priority priority = Scheduler::Default,
         Scheduler::Affinity affinity = Scheduler::WorkerThread);
    virtual void schedule(weak< Scheduler > sc) override;
};

}}  // namespace BugEngine::Task

#include <bugengine/scheduler/task/task.inl>

/**************************************************************************************************/
#endif
