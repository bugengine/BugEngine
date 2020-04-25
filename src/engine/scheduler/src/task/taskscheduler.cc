/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <taskscheduler.hh>

#include    <core/environment.hh>

#include    <scheduler/task/task.hh>
#include    <scheduler/private/taskitem.hh>
#include    <scheduler/range/sequence.hh>
#include    <settings.script.hh>


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
    ITaskItem* item = sc->pop(Scheduler::DontCare);

    if (!item)
        return false;
    item->run(sc->m_scheduler);
    be_assert(sc->hasTasks(), "running task count should be more than 1");
    return sc->taskDone();
}

intptr_t TaskScheduler::Worker::work(intptr_t p1, intptr_t p2)
{
    Worker* w = reinterpret_cast<Worker*>(p1);
    TaskScheduler* sc = reinterpret_cast<TaskScheduler*>(p2);
    while (sc->isRunning())
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
    const SchedulerSettings::Scheduler& settings = SchedulerSettings::Scheduler::get();
    const size_t g_numWorkers = settings.ThreadCount > 0 ? settings.ThreadCount
                                                         : minitl::max<i64>(1, i64(Environment::getEnvironment().getProcessorCount()) + settings.ThreadCount);
    be_info("initializing scheduler with %d workers" | g_numWorkers);
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

void TaskScheduler::queue(ITaskItem* head, ITaskItem* tail, u32 count, int priority)
{
#   if BE_ENABLE_ASSERT
    u32 debugCount = 1;
    ITaskItem* first = head;
    while (first != tail && first)
    {
        debugCount++;
        first = (ITaskItem*)first->next;
    }
    be_assert(first, "queue task does not end with specified tail");
    be_assert(tail->next == 0, "queue task tail is not the last item");
    be_assert(debugCount == count, "queue task list is %d elements, but %d indicated"
                                 | debugCount | count);
#   endif
    m_scheduler->m_runningTasks += count;
    if (head->m_owner->affinity == Scheduler::DontCare)
    {
        m_tasks[priority].pushList(head, tail);
        m_synchro.release(count);
    }
    else
    {
        m_mainThreadTasks[priority].pushList(head, tail);
        m_mainThreadSynchro.release(count);
    }
}

void TaskScheduler::queue(ITaskItem* head, ITaskItem* tail, u32 count)
{
    queue(head, tail, count, head->m_owner->priority);
}

ITaskItem* TaskScheduler::pop(Scheduler::Affinity affinity)
{
    minitl::istack<ITaskItem>* tasks = affinity == Scheduler::DontCare ? m_tasks : m_mainThreadTasks;
    for (unsigned int i = Scheduler::Immediate; i != Scheduler::Low; --i)
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
                // No split since only one main thread
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

bool TaskScheduler::taskDone()
{
	return 0 == -- m_scheduler->m_runningTasks;
}

bool TaskScheduler::hasTasks()
{
	return m_scheduler->m_runningTasks > 0;
}

bool TaskScheduler::isRunning()
{
	return m_scheduler->m_running;
}

}}
