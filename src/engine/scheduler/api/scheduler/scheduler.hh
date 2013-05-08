/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_SCHEDULER_HH_
#define BE_SCHEDULER_SCHEDULER_HH_
/*****************************************************************************/
#include    <minitl/array.hh>
#include    <minitl/pool.hh>

namespace BugEngine
{

namespace Task
{
    class TaskScheduler;
    class KernelTask;
    class TaskGroup;
    class ITask;
    class KernelTask;
    class ITaskItem;
    template< typename BODY > class Task;
    template< typename RANGE, typename BODY > class TaskItem;
}

namespace Kernel
{
    class IKernelScheduler;
    class IStream;
    struct KernelParameter;
}

class be_api(SCHEDULER) Scheduler : public minitl::pointer
{
    BE_NOCOPY(Scheduler);
    template< typename BODY > friend class Task::Task;
    template< typename RANGE, typename BODY > friend class Task::TaskItem;
    friend class Task::TaskGroup;
    friend class Task::KernelTask;
    friend class Task::TaskScheduler;
public:
    enum Priority
    {
        Low = 0,
        Default = 1,
        High = 2,
        Immediate = 3,
        PriorityCount = 4
    };
    enum Affinity
    {
        DontCare = 0,
        MainThread = 1
    };
private:
    struct WorkItem
    {
        weak<Scheduler> scheduler;
        WorkItem(weak<Scheduler> scheduler_) : scheduler(scheduler_) { scheduler->m_runningTasks++; }
        ~WorkItem() { if(0 == --scheduler->m_runningTasks) scheduler->notifyEnd(); }
    };
    friend struct WorkItem;
private:
    struct Buffer { char buffer[256]; };
    i_u32                                               m_runningTasks;
    i_bool                                              m_running;
    minitl::pool<Buffer>                                m_taskPool;
    scoped<Task::TaskScheduler>                         m_taskScheduler;
    minitl::vector< weak<Kernel::IKernelScheduler> >    m_kernelSchedulers;
private:
    void notifyEnd();
private:
    void queueTask(Task::ITaskItem* task);
    void queueKernel(weak<const Task::KernelTask> task, const minitl::array< weak<const Kernel::IStream> >& parameters);
    void* allocate(size_t size);
    void  release(void* t, size_t size);
    template< typename T > inline void* allocateTask();
    template< typename T > inline void releaseTask(T* t);
public:
    Scheduler();
    ~Scheduler();

    void mainThreadJoin();
    void addKernelScheduler(weak<Kernel::IKernelScheduler> scheduler);
    void removeKernelScheduler(weak<Kernel::IKernelScheduler> scheduler);
};

template< typename T >
void* Scheduler::allocateTask()
{
    return allocate(sizeof(T));
}

template< typename T >
void Scheduler::releaseTask(T* t)
{
    t->~T();
    release(t, sizeof(T));
}

}

/*****************************************************************************/
#endif
