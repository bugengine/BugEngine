/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_SCHEDULER_HH_
#define BE_SCHEDULER_SCHEDULER_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/minitl/array.hh>
#include <bugengine/minitl/pool.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine {

namespace Task {
class TaskScheduler;
class TaskGroup;
class ITaskItem;
class KernelTask;
}  // namespace Task

namespace KernelScheduler {
class IScheduler;
}

class be_api(SCHEDULER) Scheduler : public minitl::pointer
{
    BE_NOCOPY(Scheduler);
    friend class Task::TaskGroup;
    friend class Task::TaskScheduler;

public:
    enum Priority
    {
        Low           = 0,
        Default       = 1,
        High          = 2,
        Immediate     = 3,
        PriorityCount = 4
    };
    enum Affinity
    {
        WorkerThread = 0,
        MainThread   = 1
    };

private:
    struct WorkItem
    {
        weak< Scheduler > scheduler;
        WorkItem(weak< Scheduler > scheduler_) : scheduler(scheduler_)
        {
            scheduler->m_runningTasks++;
        }
        ~WorkItem()
        {
            if(0 == --scheduler->m_runningTasks) scheduler->notifyEnd();
        }
    };
    friend struct WorkItem;

private:
    struct Buffer
    {
        char buffer[256];
    };
    i_u32                                                 m_runningTasks;
    i_bool                                                m_running;
    minitl::pool< Buffer >                                m_taskPool;
    scoped< Task::TaskScheduler >                         m_taskScheduler;
    minitl::vector< weak< KernelScheduler::IScheduler > > m_kernelSchedulers;

private:
    void notifyEnd();

public:
    void  queueTasks(Task::ITaskItem * head, Task::ITaskItem * tail, u32 count, Priority priority);
    void  queueTasks(Task::ITaskItem * head, Task::ITaskItem * tail, u32 count);
    void  queueKernel(weak< Task::KernelTask >                                    task,
                      const minitl::array< weak< KernelScheduler::IParameter > >& parameters);
    void* allocate(size_t size);
    void  release(void* t, size_t size);
    template < typename T >
    inline void* allocateTask();
    template < typename T >
    inline void releaseTask(T * t);

public:
    Scheduler();
    ~Scheduler();

    void mainThreadJoin();
    u32  workerCount() const;
};

template < typename T >
void* Scheduler::allocateTask()
{
    return allocate(sizeof(T));
}

template < typename T >
void Scheduler::releaseTask(T* t)
{
    t->~T();
    release(t, sizeof(T));
}

}  // namespace BugEngine

/**************************************************************************************************/
#endif
