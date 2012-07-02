/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_TASKSCHEDULER_HH_
#define BE_SYSTEM_TASKSCHEDULER_HH_
/*****************************************************************************/
#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>
#include    <core/threads/event.hh>
#include    <system/scheduler/scheduler.hh>
#include    <maths/vector.hh>

namespace BugEngine
{

template< typename Body >
class Task;
class TaskGroup;
class Scheduler;

namespace ScheduledTasks
{
class ITaskItem;
}

class TaskScheduler : public minitl::pointer
{
    BE_NOCOPY(TaskScheduler);
private:
    class Worker;
    friend class Worker;
private:
    minitl::vector<Worker*> m_workers;
    Semaphore               m_synchro;
    Semaphore               m_mainThreadSynchro;
    weak<Scheduler>         m_scheduler;
private: //friend Worker
    minitl::istack<ScheduledTasks::ITaskItem>   m_tasks[Scheduler::PriorityCount];
    minitl::istack<ScheduledTasks::ITaskItem>   m_mainThreadTasks[Scheduler::PriorityCount];
public:
    ScheduledTasks::ITaskItem* pop(Scheduler::Affinity affinity);
    void queue(ScheduledTasks::ITaskItem* task);
    void split(ScheduledTasks::ITaskItem* t, size_t count);
public:
    TaskScheduler(weak<Scheduler> scheduler);
    ~TaskScheduler();

    void mainThreadJoin();
    void notifyEnd();
};

}

/*****************************************************************************/
#endif
