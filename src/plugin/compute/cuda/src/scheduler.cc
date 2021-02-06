/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <codeloader.hh>
#include <memoryhost.hh>
#include <scheduler.hh>

#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/scheduler.hh>
#include <bugengine/scheduler/task/itask.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

class CUDAKernelTaskItem : public IKernelTaskItem
{
public:
    CUDAKernelTaskItem(weak< Task::KernelTask > owner, weak< const Kernel > kernel,
                       u32 parmaeterCount);
    ~CUDAKernelTaskItem();
};

CUDAKernelTaskItem::CUDAKernelTaskItem(weak< Task::KernelTask > owner, weak< const Kernel > kernel,
                                       u32 parameterCount)
    : IKernelTaskItem(owner, kernel, parameterCount)
{
}

CUDAKernelTaskItem::~CUDAKernelTaskItem()
{
}

Scheduler::Scheduler(const Plugin::Context& context)
    : IScheduler("Cuda", context.scheduler, GPUType)
    , m_resourceManager(context.resourceManager)
    , m_cudaLoader(ref< CodeLoader >::create(Arena::task()))
    , m_memoryHost(scoped< MemoryHost >::create(Arena::task()))
{
    m_resourceManager->attach< Kernel >(m_cudaLoader);
}

Scheduler::~Scheduler()
{
    m_resourceManager->detach< Kernel >(m_cudaLoader);
}

IKernelTaskItem* Scheduler::allocateItem(weak< Task::KernelTask > owner,
                                         weak< const Kernel > kernel, u32 parameterCount)
{
    return new(Arena::temporary()) CUDAKernelTaskItem(owner, kernel, parameterCount);
}

void Scheduler::deallocateItem(CUDAKernelTaskItem* item)
{
    item->~CUDAKernelTaskItem();
    Arena::temporary().free(item);
}

void Scheduler::run(IKernelTaskItem* item)
{
    // be_notreached();
    item->owner()->completed(m_scheduler);
    deallocateItem(be_checked_cast< CUDAKernelTaskItem >(item));
}

weak< IMemoryHost > Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}  // namespace BugEngine::KernelScheduler::Cuda
