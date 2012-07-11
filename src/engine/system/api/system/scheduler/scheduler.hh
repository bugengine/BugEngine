/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
#define BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
/*****************************************************************************/

namespace BugEngine
{

namespace Task
{
    class TaskScheduler;
    template< typename BODY > class Task;
    class TaskGroup;
    class ITaskItem;
    template< typename RANGE, typename BODY > class TaskItem;
}

namespace Kernel
{
    class IKernelScheduler;
}

class be_api(SYSTEM) Scheduler : public minitl::pointer
{
    BE_NOCOPY(Scheduler);
    template< typename BODY > friend class Task::Task;
    template< typename RANGE, typename BODY > friend class Task::TaskItem;
    friend class Task::TaskGroup;
    friend class Task::TaskScheduler;
public:
    enum Priority
    {
        Low = 0,
        Default = 1,
        High = 2,
        PriorityCount = 3
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
    struct Buffer { char buffer[128]; };
    i_u32                                               m_runningTasks;
    i_bool                                              m_running;
    minitl::pool<Buffer>                                m_taskPool;
    scoped<Task::TaskScheduler>                         m_taskScheduler;
    minitl::vector< weak<Kernel::IKernelScheduler> >    m_kernelSchedulers;
private:
    void notifyEnd();
private:
    void queueTask(Task::ITaskItem* task);
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
