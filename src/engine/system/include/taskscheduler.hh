/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
public:
    ITaskItem* pop(Scheduler::Affinity affinity);
    void queue(ITaskItem* task);
    void split(ITaskItem* t, size_t count);
public:
    TaskScheduler(weak<Scheduler> scheduler);
    ~TaskScheduler();

    void mainThreadJoin();
    void notifyEnd();
};

}}

/*****************************************************************************/
#endif
