/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASKSCHEDULER_HH_
#define BE_SCHEDULER_TASKSCHEDULER_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/core/threads/event.hh>
#include <bugengine/core/threads/semaphore.hh>
#include <bugengine/core/threads/thread.hh>
#include <bugengine/kernel/interlocked_stack.hh>
#include <bugengine/kernel/simd.hh>
#include <bugengine/scheduler/scheduler.hh>

namespace BugEngine {

class Scheduler;

namespace Task {

template < typename Body >
class Task;
class TaskGroup;
class ITaskItem;

class TaskScheduler : public minitl::pointer
{
    BE_NOCOPY(TaskScheduler);

private:
    class Worker;
    friend class Worker;
    friend class ITaskItem;

private:
    minitl::vector< Worker* > m_workers;
    Semaphore                 m_synchro;
    Semaphore                 m_mainThreadSynchro;
    weak< Scheduler >         m_scheduler;

private:  // friend Worker
    minitl::istack< ITaskItem > m_tasks[Scheduler::PriorityCount];
    minitl::istack< ITaskItem > m_mainThreadTasks[Scheduler::PriorityCount];

private:  // friend class ITaskItem, Worker
    ITaskItem* pop(Scheduler::Affinity affinity);

public:
    TaskScheduler(weak< Scheduler > scheduler);
    ~TaskScheduler();

    void queue(ITaskItem* head, ITaskItem* tail, u32 count);
    void queue(ITaskItem* head, ITaskItem* tail, u32 count, int priority);

    void mainThreadJoin();
    void notifyEnd();

    bool taskDone();
    bool hasTasks();
    bool isRunning();

    u32 workerCount() const
    {
        return be_checked_numcast< u32 >(m_workers.size());
    }
};

}  // namespace Task
}  // namespace BugEngine

/**************************************************************************************************/
#endif
