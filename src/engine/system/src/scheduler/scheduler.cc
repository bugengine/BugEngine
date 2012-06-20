/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/scheduler.hh>
#include    <taskscheduler.hh>


namespace BugEngine
{

void* Scheduler::allocate(size_t size)
{
    if (size > 128)
        return new char[size];
    else
        return (void*)m_taskPool.allocate();
}

void  Scheduler::release(void* task, size_t size)
{
    if (size > 128)
        delete[] (char*)task;
    else
        m_taskPool.release((Buffer*)task);
}

Scheduler::Scheduler()
:   m_taskPool(Arena::task(), 65535, 16)
,   m_taskScheduler(scoped<TaskScheduler>::create(Arena::task(), this))
,   m_runningTasks(0)
,   m_running(true)
{
}

Scheduler::~Scheduler()
{
    m_running = false;
}

void Scheduler::queueTask(ScheduledTasks::ITaskItem* task)
{
    m_taskScheduler->queue(task);
}

void Scheduler::mainThreadJoin()
{
    m_taskScheduler->mainThreadJoin();
}

void Scheduler::notifyEnd()
{
    be_assert(m_runningTasks == 0, "should not notify end when tasks remain to be done");
    be_info("no more tasks to run; exiting");
    m_taskScheduler->notifyEnd();
}

}
