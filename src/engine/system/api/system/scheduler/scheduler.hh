/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
#define BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
/*****************************************************************************/

namespace BugEngine
{

struct PluginContext;
class TaskScheduler;
class IKernelScheduler;
template< typename BODY > class Task;
class TaskGroup;
class TaskScheduler;
namespace ScheduledTasks
{
class ITaskItem;
template< typename RANGE, typename BODY > class TaskItem;

}

class be_api(SYSTEM) Scheduler : public minitl::pointer
{
    BE_NOCOPY(Scheduler);
    template< typename BODY > friend class Task;
    template< typename RANGE, typename BODY > friend class ScheduledTasks::TaskItem;
    friend class TaskGroup;
    friend class TaskScheduler;
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
    i_u32                                       m_runningTasks;
    i_bool                                      m_running;
    minitl::pool<Buffer>                        m_taskPool;
    scoped<TaskScheduler>                       m_taskScheduler;
    minitl::vector< weak<IKernelScheduler> >    m_kernelSchedulers;
private:
    void notifyEnd();
private:
    void queueTask(ScheduledTasks::ITaskItem* task);
    void* allocate(size_t size);
    void  release(void* t, size_t size);
    template< typename T > inline void* allocateTask();
    template< typename T > inline void releaseTask(T* t);
public:
    Scheduler();
    ~Scheduler();

    void mainThreadJoin();
    void addKernelScheduler(weak<IKernelScheduler> scheduler);
    void removeKernelScheduler(weak<IKernelScheduler> scheduler);
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
