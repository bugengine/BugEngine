/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/scheduler.hh>
#include    <system/scheduler/kernel/ikernelscheduler.hh>
#include    <system/scheduler/kernel/cpumemoryprovider.hh>
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
    :   m_runningTasks(0)
    ,   m_running(true)
    ,   m_taskPool(Arena::task(), 65535, 16)
    ,   m_taskScheduler(scoped<Task::TaskScheduler>::create(Arena::task(), this))
    ,   m_kernelSchedulers(Arena::task())
    ,   m_cpuMemoryProvider(scoped<Kernel::CPUMemoryProvider>::create(Arena::task()))
{
}

Scheduler::~Scheduler()
{
    m_running = false;
}

void Scheduler::queueTask(Task::ITaskItem* task)
{
    m_taskScheduler->queue(task);
}

void Scheduler::queueKernel()
{
    be_assert(m_kernelSchedulers.size() > 0, "no kernel scheduler installed");
    
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

}
