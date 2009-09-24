/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
#define BE_SYSTEM_SCHEDULER_SCHEDULER_HH_
/*****************************************************************************/
#include    <core/threads/thread.hh>
#include    <core/utils/singleton.hh>
#include    <core/threads/semaphore.hh>
#include    <core/threads/event.hh>
#include    <minitl/memory/pool.hh>
#include    <core/timer.hh>
#include    <maths/vector.hh>

namespace BugEngine
{

template< typename Body >
class Task;

namespace ScheduledTasks
{

class BaseTaskItem;

}

class be_api(CORE) Scheduler
{
private:
    class Worker;
    class HeadTask;
    friend class Worker;
    friend class ScheduledTasks::BaseTaskItem;
    template< typename Body >
    friend class Task;
private:
    std::vector< Worker* >          m_workers;
    Semaphore                       m_synchro;
    minitl::pool< char[128] >       m_taskPool;
    unsigned int                    m_frameCount;
    Timer                           m_timer;
    tick_type                       m_lastFrameTick;
    tick_type                       m_currentFrameTick;
    float                           m_frameTime;
    Event                           m_end;
private: //friend Worker
    minitl::istack<ScheduledTasks::BaseTaskItem>    m_tasks;
    i_u32                                           m_runningTasks;
    bool volatile                                   m_running;
private:
    void split(ScheduledTasks::BaseTaskItem* t, size_t count);
private: //friend Worker
    ScheduledTasks::BaseTaskItem* pop();
    void queue(ScheduledTasks::BaseTaskItem* task);
public:
    void* allocate_task(size_t size);
    void  release_task(void* t, size_t size);
    template< typename T >
    void* allocate_task()
    {
        return allocate_task(sizeof(T));
    }
    template< typename T >
    void release_task(T* t)
    {
        t->~T();
        return release_task(reinterpret_cast<void*>(static_cast<ScheduledTasks::BaseTaskItem*>(t)), sizeof(T));
    }
    void run(ScheduledTasks::BaseTaskItem* task);
public:
    Scheduler();
    ~Scheduler();

    void frameUpdate();
    void wait();
private:
    Scheduler(const Scheduler& other);
    Scheduler& operator=(const Scheduler& other);
};

}

/*****************************************************************************/
#endif
