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

void Scheduler::run(weak< Task::KernelTask > task, weak< const Kernel > kernel,
                    const minitl::array< weak< const IMemoryBuffer > >& parameters)
{
    be_forceuse(kernel);
    be_forceuse(parameters);
    // be_notreached();
    task->completed(m_scheduler);
}

weak< IMemoryHost > Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
