/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin.compute.opencl/memoryhost.hh>
#include <bugengine/plugin.compute.opencl/scheduler.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/scheduler.hh>
#include <bugengine/scheduler/task/itask.hh>
#include <codeloader.hh>
#include <context.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

static const int s_profilingMode =
#if BE_OPTIM_LEVEL_AT_MOST(BE_OPTIM_LVEL_PROFILE)
    CL_QUEUE_PROFILING_ENABLE
#else
    0
#endif
    ;

class CLKernelTaskItem : public IKernelTaskItem
{
public:
    CLKernelTaskItem(weak< Task::KernelTask > owner, weak< const Kernel > kernel,
                     u32 parmaeterCount);
    ~CLKernelTaskItem();
};

CLKernelTaskItem::CLKernelTaskItem(weak< Task::KernelTask > owner, weak< const Kernel > kernel,
                                   u32 parameterCount)
    : IKernelTaskItem(owner, kernel, parameterCount)
{
}

CLKernelTaskItem::~CLKernelTaskItem()
{
}

Scheduler::Scheduler(const Plugin::Context& pluginContext, ref< Context > clContext)
    : IScheduler("OpenCL", pluginContext.scheduler, GPUType)
    , m_context(clContext)
    , m_resourceManager(pluginContext.resourceManager)
    , m_loader(scoped< CodeLoader >::create(Arena::task(), m_context))
    , m_memoryHost(scoped< MemoryHost >::create(Arena::task()))
    , m_errorCode(0)
    , m_commandQueue(clCreateCommandQueue(m_context->m_context, m_context->m_device,
                                          s_profilingMode, &m_errorCode))
{
    if(m_context->m_context)
    {
        m_resourceManager->attach< Kernel >(weak< Resource::ILoader >(m_loader));
    }
}

Scheduler::~Scheduler()
{
    if(m_context->m_context)
    {
        m_resourceManager->detach< Kernel >(weak< const Resource::ILoader >(m_loader));
        clReleaseCommandQueue(m_commandQueue);
    }
}

IKernelTaskItem* Scheduler::allocateItem(weak< Task::KernelTask > owner,
                                         weak< const Kernel > kernel, u32 parameterCount)
{
    return new(Arena::temporary()) CLKernelTaskItem(owner, kernel, parameterCount);
}

void Scheduler::deallocateItem(CLKernelTaskItem* item)
{
    item->~CLKernelTaskItem();
    Arena::temporary().free(item);
}

void Scheduler::run(IKernelTaskItem* item)
{
    // be_notreached();
    item->owner()->completed(m_scheduler);
    deallocateItem(be_checked_cast< CLKernelTaskItem >(item));
}

weak< IMemoryHost > Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
