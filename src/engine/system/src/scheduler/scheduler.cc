/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <core/environment.hh>
#include    <core/settings/manager.hh>
#include    <system/scheduler/scheduler.hh>
#include    <core/threads/mutex.hh>
#include    <system/scheduler/task/task.hh>
#include    <system/scheduler/private/taskitem.hh>
#include    <system/scheduler/range/sequence.hh>
#include    <minitl/interlocked/stack.hh>
#include    <maths/vector.hh>


namespace BugEngine
{

class Scheduler::Worker
{
protected:
    static const int s_frameCount = 8;
private:
    size_t const                                m_workerId;
    Thread                                      m_workThread;
protected:
    void unhook(ScheduledTasks::ITaskItem* prev, ScheduledTasks::ITaskItem* t);
public:
    Worker(Scheduler* scheduler, size_t workerId);
    ~Worker();

    bool doWork(Scheduler* sc);

    void frameUpdate();

    static intptr_t work(intptr_t p1, intptr_t p2);
private:
    Worker& operator=(const Worker& other);
    Worker(const Worker& other);
};

Scheduler::Worker::Worker(Scheduler* scheduler, size_t workerId)
:   m_workerId(workerId)
,   m_workThread(istring(minitl::format<>("worker %u") | workerId), &Scheduler::Worker::work, reinterpret_cast<intptr_t>(this), reinterpret_cast<intptr_t>(scheduler), Thread::Highest)
{
}

Scheduler::Worker::~Worker()
{
}

bool Scheduler::Worker::doWork(Scheduler* sc)
{
    static const i32& s_taskCount = 16;

    ScheduledTasks::ITaskItem* target = sc->pop();
    if(!target)
        return false;
    if(!target->atomic() && 1l << target->m_splitCount <= s_taskCount)
    {
        ScheduledTasks::ITaskItem* newTarget = target->split(sc);
        sc->queue(newTarget);
        sc->queue(target);
    }
    else
    {
        target->run(sc);
    }
    be_assert(sc->m_runningTasks > 0, "running task count should be more than 1");
    return --sc->m_runningTasks == 0;
}


void Scheduler::Worker::frameUpdate()
{
}

intptr_t Scheduler::Worker::work(intptr_t p1, intptr_t p2)
{
    Worker* w = reinterpret_cast<Worker*>(p1);
    Scheduler* sc = reinterpret_cast<Scheduler*>(p2);
    while(sc->m_running)
    {
        if(sc->m_synchro.wait() == Threads::Waitable::Finished)
            if(w->doWork(sc))
                sc->m_end.set();
    }
    return 0;
}




void* Scheduler::allocate_task(size_t size)
{
    if(size > 128)
        return new char[size];
    else
        return (void*)m_taskPool.allocate();
}

void  Scheduler::release_task(void* task, size_t size)
{
    if(size > 128)
        delete[] (char*)task;
    else
        m_taskPool.release((char(*)[128])task);
}

Scheduler::Scheduler()
:   m_workers(gameArena())
,   m_synchro(0, 65535)
,   m_taskPool(gameArena(), 65535, 16)
,   m_frameCount(0)
,   m_end()
,   m_tasks()
,   m_runningTasks(0)
,   m_running(true)
{
    const size_t g_numWorkers = Environment::getEnvironment().getProcessorCount();
    for(size_t i = 0; i < g_numWorkers; ++i)
    {
        m_workers.push_back(new Worker(this, i));
    }
}

Scheduler::~Scheduler()
{
    m_running = false;
    m_synchro.release((int)m_workers.size());
    for(size_t i = 0; i < m_workers.size(); ++i)
        delete m_workers[i];
}

void Scheduler::frameUpdate()
{
    m_timer.stop();
    //float time = Timer::now();
    //send(time)
    for(size_t i = 0; i < m_workers.size(); ++i)
    {
        m_workers[i]->frameUpdate();
    }
    m_frameCount++;
    m_timer.reset();
    m_timer.start();
}

void Scheduler::queue(ScheduledTasks::ITaskItem* task)
{
    int priority = task->m_owner->priority;
    m_tasks[priority].push(task);
    m_runningTasks ++;
    m_synchro.release(1);
}

ScheduledTasks::ITaskItem* Scheduler::pop()
{
    for(unsigned int i = High; i != Low; --i)
    {
        ScheduledTasks::ITaskItem* t = m_tasks[i].pop();
        if(t)
            return t;
    }
    return 0;
}

void Scheduler::wait()
{
    while(m_runningTasks)
    {
        m_end.lock();
        m_end.wait();
        m_end.unlock();
    }
}

}
