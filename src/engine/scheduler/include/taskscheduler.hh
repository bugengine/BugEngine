/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASKSCHEDULER_HH_
#define BE_SCHEDULER_TASKSCHEDULER_HH_
/*****************************************************************************/
#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>
#include    <core/threads/event.hh>
#include    <scheduler/scheduler.hh>
#include    <kernel/interlocked_stack.hh>
#include    <kernel/simd.hh>

namespace BugEngine
{

class Scheduler;

namespace Task
{

template< typename Body > class Task;
class TaskGroup;
class ITaskItem;

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
    minitl::istack<ITaskItem>   m_tasks[Scheduler::PriorityCount];
    minitl::istack<ITaskItem>   m_mainThreadTasks[Scheduler::PriorityCount];
private:
    void queue(ITaskItem* task, int affinity);
    ITaskItem* pop(Scheduler::Affinity affinity);
    void split(ITaskItem* t, size_t count);
public:
    TaskScheduler(weak<Scheduler> scheduler);
    ~TaskScheduler();

    void queue(ITaskItem* task);

    void mainThreadJoin();
    void notifyEnd();

	bool taskDone();
	bool hasTasks();
	bool isRunning();
};

}}

/*****************************************************************************/
#endif
