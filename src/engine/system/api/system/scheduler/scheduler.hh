/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
#define BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
/*****************************************************************************/
#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>
#include    <core/threads/event.hh>
#include    <minitl/memory/pool.hh>
#include    <core/timer.hh>
#include    <maths/vector.hh>

namespace BugEngine
{

template< typename Body >
class Task;
class TaskGroup;

namespace ScheduledTasks
{
class ITaskItem;
}

class be_api(SYSTEM) Scheduler : public minitl::pointer
{
    BE_NOCOPY(Scheduler);
private:
    class Worker;
    class HeadTask;
    friend class Worker;
    friend class ScheduledTasks::ITaskItem;
    template< typename Body >
    friend class Task;
    friend class TaskGroup;
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
    minitl::vector<Worker*>                     m_workers;
    Semaphore                                   m_synchro;
    Semaphore                                   m_mainThreadSynchro;
    struct Buffer { char buffer[128]; };
    minitl::pool<Buffer>                        m_taskPool;
    unsigned int                                m_frameCount;
    Timer                                       m_timer;
    Event                                       m_end;
private: //friend Worker
    minitl::istack<ScheduledTasks::ITaskItem>   m_tasks[PriorityCount];
    minitl::istack<ScheduledTasks::ITaskItem>   m_mainThreadTasks[PriorityCount];
    i_u32                                       m_runningTasks;
    bool volatile                               m_running;
private:
    void notifyEnd();
    ScheduledTasks::ITaskItem* pop(Affinity affinity);
    void queue(ScheduledTasks::ITaskItem* task);
    void split(ScheduledTasks::ITaskItem* t, size_t count);
    void* allocate_task(size_t size);
    void  release_task(void* t, size_t size);
public:
    template< typename T > inline void* allocate_task();
    template< typename T > inline void release_task(T* t);
public:
    Scheduler();
    ~Scheduler();

    void frameUpdate();
    void mainThreadJoin();
};

template< typename T >
void* Scheduler::allocate_task()
{
    return allocate_task(sizeof(T));
}
template< typename T >
void Scheduler::release_task(T* t)
{
    t->~T();
    return release_task(reinterpret_cast<void*>(static_cast<ScheduledTasks::ITaskItem*>(t)), sizeof(T));
}

}

/*****************************************************************************/
#endif
