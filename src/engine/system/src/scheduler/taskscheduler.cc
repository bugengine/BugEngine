/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <taskscheduler.hh>

#include    <core/environment.hh>

#include    <system/scheduler/task/task.hh>
#include    <system/scheduler/private/taskitem.hh>
#include    <system/scheduler/range/sequence.hh>


namespace BugEngine { namespace Task
{

class TaskScheduler::Worker
{
    BE_NOCOPY(Worker);
protected:
    static const int s_frameCount = 8;
private:
    size_t const                                m_workerId;
    Thread                                      m_workThread;
protected:
    void unhook(ITaskItem* prev, ITaskItem* t);
public:
    Worker(weak<TaskScheduler> scheduler, size_t workerId);
    ~Worker();

    bool doWork(weak<TaskScheduler> sc);

    static intptr_t work(intptr_t p1, intptr_t p2);
};

TaskScheduler::Worker::Worker(weak<TaskScheduler> scheduler, size_t workerId)
:   m_workerId(workerId)
,   m_workThread(
        istring(minitl::format<128u>("worker %u") | workerId),
        &TaskScheduler::Worker::work, reinterpret_cast<intptr_t>(this),
        reinterpret_cast<intptr_t>(scheduler.operator->()),
        Thread::BelowNormal)
{
}

TaskScheduler::Worker::~Worker()
{
}

bool TaskScheduler::Worker::doWork(weak<TaskScheduler> sc)
{
    static const i32& s_taskCount = 16;

    ITaskItem* target = sc->pop(Scheduler::DontCare);
    if (!target)
        return false;
    if (!target->atomic() && 1l << target->m_splitCount <= s_taskCount)
    {
        ITaskItem* newTarget = target->split(sc->m_scheduler);
        sc->queue(newTarget);
        sc->queue(target);
    }
    else
    {
        target->run(sc->m_scheduler);
    }
    be_assert(sc->m_scheduler->m_runningTasks > 0, "running task count should be more than 1");
    return --sc->m_scheduler->m_runningTasks == 0;
}

intptr_t TaskScheduler::Worker::work(intptr_t p1, intptr_t p2)
{
    Worker* w = reinterpret_cast<Worker*>(p1);
    TaskScheduler* sc = reinterpret_cast<TaskScheduler*>(p2);
    while (sc->m_scheduler->m_running)
    {
        if (sc->m_synchro.wait() == Threads::Waitable::Finished)
        {
            if (w->doWork(sc))
            {
                sc->notifyEnd();
            }
        }
    }
    return 0;
}


TaskScheduler::TaskScheduler(weak<Scheduler> scheduler)
:   m_workers(Arena::task())
,   m_synchro(0)
,   m_mainThreadSynchro(0)
,   m_scheduler(scheduler)
,   m_tasks()
,   m_mainThreadTasks()
{
    const size_t g_numWorkers = Environment::getEnvironment().getProcessorCount();
    for (size_t i = 0; i < g_numWorkers; ++i)
    {
        m_workers.push_back(new Worker(this, i));
    }
}

TaskScheduler::~TaskScheduler()
{
    m_synchro.release((int)m_workers.size());
    m_mainThreadSynchro.release(1);
    for (size_t i = 0; i < m_workers.size(); ++i)
        delete m_workers[i];
}

void TaskScheduler::queue(ITaskItem* task)
{
    int priority = task->m_owner->priority;
    m_scheduler->m_runningTasks ++;
    if (task->m_owner->affinity == Scheduler::DontCare)
    {
        m_tasks[priority].push(task);
        m_synchro.release(1);
    }
    else
    {
        m_mainThreadTasks[priority].push(task);
        m_mainThreadSynchro.release(1);
    }
}

ITaskItem* TaskScheduler::pop(Scheduler::Affinity affinity)
{
    minitl::istack<ITaskItem>* tasks = affinity == Scheduler::DontCare ? m_tasks : m_mainThreadTasks;
    for (unsigned int i = Scheduler::High; i != Scheduler::Low; --i)
    {
        ITaskItem* t = tasks[i].pop();
        if (t)
            return t;
    }
    return 0;
}

void TaskScheduler::mainThreadJoin()
{
    while (true)
    {
        if (m_mainThreadSynchro.wait() == Threads::Waitable::Finished)
        {
            ITaskItem* t = pop(Scheduler::MainThread);
            if (t)
            {
                t->run(m_scheduler);
                if (--m_scheduler->m_runningTasks == 0)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
}

void TaskScheduler::notifyEnd()
{
    m_mainThreadSynchro.release(1);
}

}}
