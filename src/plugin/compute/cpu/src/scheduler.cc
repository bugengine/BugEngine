/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <codeloader.hh>
#include <kernelloader.hh>
#include <memoryhost.hh>
#include <scheduler.hh>

#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/scheduler.hh>
#include <bugengine/scheduler/task/kerneltask.hh>
#include <bugengine/scheduler/task/task.hh>
#include <kernel_optims.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

class CPUTaskItem;

class CPUKernelTaskItem : public IKernelTaskItem
{
    friend class Scheduler;

private:
    weak< Scheduler >    m_cpuScheduler;
    weak< KernelObject > m_object;
    const u32            m_jobCount;
    i_u32                m_doneCount;

public:
    CPUKernelTaskItem(weak< Task::KernelTask > owner, weak< const Kernel > kernel,
                      weak< Scheduler > scheduler, weak< KernelObject > object, u32 parameterCount,
                      u32 jobCount);
    ~CPUKernelTaskItem();

    void onJobCompleted(weak< BugEngine::Scheduler > sc);

    weak< KernelObject > object() const
    {
        return m_object;
    }
};

CPUKernelTaskItem::CPUKernelTaskItem(weak< Task::KernelTask > owner, weak< const Kernel > kernel,
                                     weak< Scheduler > scheduler, weak< KernelObject > object,
                                     u32 parameterCount, u32 jobCount)
    : IKernelTaskItem(owner, kernel, parameterCount)
    , m_cpuScheduler(scheduler)
    , m_object(object)
    , m_jobCount(jobCount)
    , m_doneCount(i_u32::create(0))
{
}

CPUKernelTaskItem::~CPUKernelTaskItem()
{
}

void CPUKernelTaskItem::onJobCompleted(weak< BugEngine::Scheduler > sc)
{
    if(++m_doneCount == m_jobCount)
    {
        m_owner->completed(sc);
        m_cpuScheduler->deallocateItem(this);
    }
}

class CPUTaskItem : public Task::ITaskItem
{
private:
    CPUKernelTaskItem* m_kernelItem;
    u32                m_index;
    u32                m_total;

public:
    virtual void run(weak< BugEngine::Scheduler > sc) override;

    CPUTaskItem(CPUKernelTaskItem* item, u32 index, u32 total);
};

CPUTaskItem::CPUTaskItem(CPUKernelTaskItem* item, u32 index, u32 total)
    : ITaskItem(item->owner())
    , m_kernelItem(item)
    , m_index(index)
    , m_total(total)
{
}

void CPUTaskItem::run(weak< BugEngine::Scheduler > sc)
{
    m_kernelItem->object()->run(m_index, m_total, m_kernelItem->parameters());
    m_kernelItem->onJobCompleted(sc);
    this->release< CPUTaskItem >(sc);
}

Scheduler::Scheduler(const Plugin::Context& context)
    : IScheduler("CPU", context.scheduler, CPUType)
    , m_resourceManager(context.resourceManager)
    , m_cpuLoaders(Arena::task(), s_cpuVariantCount + 1)
    , m_memoryHost(scoped< MemoryHost >::create(Arena::task()))
{
    for(i32 i = 0; i < s_cpuVariantCount; ++i)
    {
        if(*s_cpuVariants[i])
            be_info("registering optimised CPU kernel loader for %s" | s_cpuVariants[i]);
        else
            be_info("registering unoptimised CPU kernel loader");
        ref< CodeLoader > codeLoader
            = ref< CodeLoader >::create(Arena::task(), inamespace(s_cpuVariants[i]));
        m_cpuLoaders.push_back(ref< KernelLoader >::create(Arena::task(), codeLoader));
        m_resourceManager->attach< Code >(codeLoader);
        m_resourceManager->attach< Kernel >(m_cpuLoaders[i]);
    }
}

Scheduler::~Scheduler()
{
    for(minitl::vector< ref< KernelLoader > >::const_reverse_iterator it = m_cpuLoaders.rbegin();
        it != m_cpuLoaders.rend(); ++it)
    {
        m_resourceManager->detach< Kernel >(*it);
        m_resourceManager->detach< Code >((*it)->codeLoader());
    }
}

IKernelTaskItem* Scheduler::allocateItem(weak< Task::KernelTask > owner,
                                         weak< const Kernel > kernel, u32 parameterCount)
{
    u32                  jobCount = m_scheduler->workerCount() * /* TODO: settings */ 4;
    weak< KernelObject > object
        = kernel->getResource(m_cpuLoaders[0]).getRefHandle< KernelObject >();
    be_assert(object, "kernel is not loaded");
    return new(Arena::temporary()) /* TODO: pool */
        CPUKernelTaskItem(owner, kernel, this, object, parameterCount, jobCount);
}

void Scheduler::deallocateItem(CPUKernelTaskItem* item)
{
    item->~CPUKernelTaskItem();
    Arena::temporary().free(item);
}

void Scheduler::run(IKernelTaskItem* item)
{
    /* TODO: set option to use Neon/AVX/SSE */
    CPUKernelTaskItem* cpuItem = be_checked_cast< CPUKernelTaskItem >(item);
    CPUTaskItem *      head = 0, *tail = 0;
    for(u32 i = 0, jobCount = cpuItem->m_jobCount; i < jobCount; ++i)
    {
        CPUTaskItem* taskItem
            = new(m_scheduler->allocateTask< CPUTaskItem >()) CPUTaskItem(cpuItem, i, jobCount);
        if(!tail) tail = taskItem;
        taskItem->next = head;
        head           = taskItem;
    }
    m_scheduler->queueTasks(head, tail, cpuItem->m_jobCount);
}

weak< IMemoryHost > Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}  // namespace BugEngine::KernelScheduler::CPU
