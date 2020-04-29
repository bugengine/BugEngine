/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/scheduler.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/kernel/ischeduler.hh>
#include    <scheduler/kernel/imemorybuffer.hh>
#include    <scheduler/kernel/ischeduler.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <taskscheduler.hh>
#include    <rtti/engine/namespace.hh>


namespace BugEngine
{

void* Scheduler::allocate(size_t size)
{
    if (size > 128)
        return Arena::task().alloc(size, 1);
    else
        return (void*)m_taskPool.allocate();
}

void  Scheduler::release(void* task, size_t size)
{
    if (size > 128)
        Arena::task().free(task);
    else
        m_taskPool.release((Buffer*)task);
}

Scheduler::Scheduler()
    :   m_runningTasks(i_u32::create(0))
    ,   m_running(i_bool::create(true))
    ,   m_taskPool(Arena::task(), 65535, 16)
    ,   m_taskScheduler(scoped<Task::TaskScheduler>::create(Arena::task(), this))
    ,   m_kernelSchedulers(Arena::task())
{
}

Scheduler::~Scheduler()
{
    m_running = false;
}

void Scheduler::queueTasks(Task::ITaskItem* head, Task::ITaskItem* tail,
                           u32 count, Priority priority)
{
    m_taskScheduler->queue(head, tail, count, priority);
}

void Scheduler::queueTasks(Task::ITaskItem* head, Task::ITaskItem* tail, u32 count)
{
    m_taskScheduler->queue(head, tail, count);
}

void Scheduler::queueKernel(weak<Task::KernelTask> task,
                            const minitl::array< weak<KernelScheduler::IParameter> >& parameters)
{
    u32 paramCount = parameters.size();
    minitl::array< weak<const KernelScheduler::IMemoryBuffer> > kernelParams(Arena::temporary(), paramCount);
    weak<KernelScheduler::IScheduler> scheduler = task->m_targetScheduler;
    weak<KernelScheduler::IMemoryHost> memHost = scheduler->memoryHost();
    for (u32 i = 0; i < paramCount; ++i)
    {
        weak<const KernelScheduler::IMemoryBuffer> bank = parameters[i]->getBank(memHost);
        kernelParams[i] = bank;
    }
    scheduler->run(task, task->m_kernel, kernelParams);
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

u32 Scheduler::workerCount() const
{
    return m_taskScheduler->workerCount();
}

}
