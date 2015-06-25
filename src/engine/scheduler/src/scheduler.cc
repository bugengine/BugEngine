/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/scheduler.hh>
#include    <scheduler/kernel/ikernelscheduler.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/kernel/istream.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <taskscheduler.hh>


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
    :   m_runningTasks(i_u32::Zero)
    ,   m_running(i_bool::One)
    ,   m_taskPool(Arena::task(), 65535, 16)
    ,   m_taskScheduler(scoped<Task::TaskScheduler>::create(Arena::task(), this))
    ,   m_kernelSchedulers(Arena::task())
{
}

Scheduler::~Scheduler()
{
    m_running = false;
}

void Scheduler::queueTask(Task::ITaskItem* task, Priority priority)
{
    m_taskScheduler->queue(task, priority);
}

void Scheduler::queueTask(Task::ITaskItem* task)
{
    m_taskScheduler->queue(task);
}

void Scheduler::queueKernel(weak<Task::KernelTask> task, const minitl::array< weak<const Kernel::IStream> >& parameters)
{
    be_assert(m_kernelSchedulers.size() > 0, "no kernel scheduler installed");
    u32 paramCount = parameters.size();
    minitl::array<Kernel::KernelParameter> kernelParams(Arena::temporary(), paramCount);
    for (u32 i = 0; i < paramCount; ++i)
    {
        weak<const Kernel::IMemoryBank> bank = parameters[i]->getCurrentBank();
        if (bank)
        {
            kernelParams[i] = bank->getKernelParameter();
        }
        else
        {
            kernelParams[i].p1 = 0;
            kernelParams[i].p2 = 0;
        }
    }
    m_kernelSchedulers[0]->run(task, task->m_kernel, kernelParams);
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

void Scheduler::addKernelScheduler(weak<Kernel::IKernelScheduler> scheduler)
{
    m_kernelSchedulers.push_back(scheduler);
}

void Scheduler::removeKernelScheduler(weak<Kernel::IKernelScheduler> scheduler)
{
    for (minitl::vector< weak<Kernel::IKernelScheduler> >::iterator it = m_kernelSchedulers.begin(); it != m_kernelSchedulers.end(); ++it)
    {
        if (*it == scheduler)
        {
            m_kernelSchedulers.erase(it);
            return;
        }
    }
    be_notreached();
}

u32 Scheduler::workerCount() const
{
    return m_taskScheduler->workerCount();
}

}
