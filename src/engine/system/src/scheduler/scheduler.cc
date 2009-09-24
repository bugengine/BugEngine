/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <core/environment.hh>
#include    <core/settings/manager.hh>
#include    <system/scheduler/scheduler.hh>
#include    <core/threads/mutex.hh>
#include    <system/scheduler/task.hh>
#include    <system/scheduler/taskitem.hh>
#include    <system/scheduler/range/sequence.hh>
#include    <minitl/interlocked/stack.hh>
#include    <maths/vector.hh>


namespace BugEngine
{

class Scheduler::Worker
{
protected:
    struct WorkReport : public minitl::inode
    {
        const BaseTask* task;
        tick_type       start;
        tick_type       stop;
    };
protected:
    static const int s_frameCount = 8;
private:
    size_t const                            m_workerId;
    WorkReport*                             m_frames[s_frameCount];
    volatile size_t                         m_currentFrame;
    minitl::pool<WorkReport>                m_reportPool;
    Thread                                  m_workThread;
protected:
    void unhook(ScheduledTasks::BaseTaskItem* prev, ScheduledTasks::BaseTaskItem* t);
public:
    Worker(Scheduler* scheduler, size_t workerId);
    ~Worker();

    bool doWork(Scheduler* sc);

    WorkReport* startTimer(const BaseTask* t);
    void stopTimer(WorkReport* r);

    void frameUpdate();

    static intptr_t work(intptr_t p1, intptr_t p2);
private:
    Worker& operator=(const Worker& other);
    Worker(const Worker& other);
};

Scheduler::Worker::Worker(Scheduler* scheduler, size_t workerId)
:   m_workerId(workerId)
,   m_currentFrame(0)
,   m_reportPool(1024*s_frameCount)
,   m_workThread(istring(minitl::format<>("worker %u") | workerId), &Scheduler::Worker::work, reinterpret_cast<intptr_t>(this), reinterpret_cast<intptr_t>(scheduler), Thread::Highest)
{
    for(int i = 0; i < s_frameCount; ++i)
        m_frames[i] = 0;
}

Scheduler::Worker::~Worker()
{
}

bool Scheduler::Worker::doWork(Scheduler* sc)
{
    SettingsManager::getManager();
    static const i32& s_taskCount = 16;

    ScheduledTasks::BaseTaskItem* target = sc->pop();
    target->m_currentState = ScheduledTasks::BaseTaskItem::Executing;
    if(!target->atomic() && 1l << target->m_splitCount <= s_taskCount)
    {
        ScheduledTasks::BaseTaskItem* newTarget = target->split(sc);
        sc->queue(newTarget);
        sc->queue(target);
    }
    else
    {
        WorkReport* r = startTimer(target->m_owner);
        target->run(sc);
        stopTimer(r);
    }
    be_assert(sc->m_runningTasks > 0, "running task count should be more than 1");
    return --sc->m_runningTasks == 0;
}


Scheduler::Worker::WorkReport* Scheduler::Worker::startTimer(const BaseTask* t)
{
    WorkReport* r = m_reportPool.allocate();
    r->task = t;
    r->start = tick();
    return r;
}

void Scheduler::Worker::stopTimer(WorkReport* r)
{
    r->stop = tick();
    size_t currentFrame = m_currentFrame;
    r->next = m_frames[currentFrame];
    m_frames[currentFrame] = r;
}

void Scheduler::Worker::frameUpdate()
{
    size_t previousFrame = (m_currentFrame+1) % s_frameCount;
    WorkReport* r = m_frames[previousFrame];
    while(r)
    {
        WorkReport* old = r;
        r = static_cast<WorkReport*>((minitl::inode*)r->next);
        m_reportPool.release(old);
    }
    m_frames[previousFrame] = m_reportPool.allocate();
    m_frames[previousFrame]->task = 0;
    m_frames[previousFrame]->start = tick();
    m_frames[previousFrame]->stop = m_frames[previousFrame]->start;
    m_frames[previousFrame]->next = 0;
    m_currentFrame = previousFrame;
}

intptr_t Scheduler::Worker::work(intptr_t p1, intptr_t p2)
{
    Worker* w = reinterpret_cast<Worker*>(p1);
    Scheduler* sc = reinterpret_cast<Scheduler*>(p2);
    while(sc->m_running)
    {
        if(sc->m_synchro.wait(10) == Threads::Waitable::Finished)
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
:   m_workers()
,   m_synchro(0, 65535)
,   m_taskPool(65535, 16)
,   m_frameCount(0)
,   m_lastFrameTick(m_currentFrameTick)
,   m_currentFrameTick(tick())
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
    for(size_t i = 0; i < m_workers.size(); ++i)
        delete m_workers[i];
}

void Scheduler::frameUpdate()
{
    m_lastFrameTick = m_currentFrameTick;
    m_currentFrameTick = tick();
    be_assert(m_currentFrameTick >= m_lastFrameTick, "processor clock seems to be in the past");
    m_frameTime = m_timer.tick();
    for(size_t i = 0; i < m_workers.size(); ++i)
    {
        m_workers[i]->frameUpdate();
    }
    m_frameCount++;
}

void Scheduler::queue(ScheduledTasks::BaseTaskItem* task)
{
    m_tasks.push(task);
    m_runningTasks ++;
    m_synchro.release(1);
}

ScheduledTasks::BaseTaskItem* Scheduler::pop()
{
    return m_tasks.pop();
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
