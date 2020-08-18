/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/rtti/engine/namespace.hh>
#include <bugengine/scheduler/kernel/imemorybuffer.hh>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/scheduler.hh>
#include <bugengine/scheduler/task/kerneltask.hh>
#include <taskscheduler.hh>

namespace BugEngine {

void* Scheduler::allocate(size_t size)
{
    if(size > 128)
        return Arena::task().alloc(size, 1);
    else
        return (void*)m_taskPool.allocate();
}

void Scheduler::release(void* task, size_t size)
{
    if(size > 128)
        Arena::task().free(task);
    else
        m_taskPool.release((Buffer*)task);
}

Scheduler::Scheduler()
    : m_runningTasks(i_u32::create(0))
    , m_running(i_bool::create(true))
    , m_taskPool(Arena::task(), 65535, 16)
    , m_taskScheduler(scoped< Task::TaskScheduler >::create(Arena::task(), this))
    , m_kernelSchedulers(Arena::task())
{
}

Scheduler::~Scheduler()
{
    m_running = false;
}

void Scheduler::queueTasks(Task::ITaskItem* head, Task::ITaskItem* tail, u32 count,
                           Priority priority)
{
    m_taskScheduler->queue(head, tail, count, priority);
}

void Scheduler::queueTasks(Task::ITaskItem* head, Task::ITaskItem* tail, u32 count)
{
    m_taskScheduler->queue(head, tail, count);
}

void Scheduler::queueKernel(weak< Task::KernelTask >                                    task,
                            const minitl::array< weak< KernelScheduler::IParameter > >& parameters)
{
    u32                                                           paramCount = parameters.size();
    minitl::array< weak< const KernelScheduler::IMemoryBuffer > > kernelParams(Arena::temporary(),
                                                                               paramCount);
    weak< KernelScheduler::IScheduler >  scheduler = task->m_targetScheduler;
    weak< KernelScheduler::IMemoryHost > memHost   = scheduler->memoryHost();
    KernelScheduler::IKernelTaskItem*    item
        = scheduler->allocateItem(task, task->m_kernel, paramCount);
    for(u32 i = 0; i < paramCount; ++i)
    {
        item->m_parameters[i] = parameters[i]->getBank(memHost);
    }
    scheduler->run(item);
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

}  // namespace BugEngine
