/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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

class DebugRenderer;
template< typename Body >
class Task;

namespace ScheduledTasks
{

class BaseTaskItem;

}

class COREEXPORT Scheduler
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
    void draw(DebugRenderer* renderer, int2 start, int2 stop, float framesPerScreen) const;
private:
    Scheduler(const Scheduler& other);
    Scheduler& operator=(const Scheduler& other);
};

}

/*****************************************************************************/
#endif
