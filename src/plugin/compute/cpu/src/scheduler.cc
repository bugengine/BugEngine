/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <codeloader.hh>
#include <memoryhost.hh>
#include <scheduler.hh>

#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/scheduler.hh>
#include <bugengine/scheduler/task/itask.hh>
#include <kernel_optims.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

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
        m_cpuLoaders.push_back(
           ref< CodeLoader >::create(Arena::task(), inamespace(s_cpuVariants[i])));
        m_resourceManager->attach< Kernel >(m_cpuLoaders[i]);
    }
}

Scheduler::~Scheduler()
{
    for(minitl::vector< ref< CodeLoader > >::const_reverse_iterator it = m_cpuLoaders.rbegin();
        it != m_cpuLoaders.rend(); ++it)
    {
        m_resourceManager->detach< Kernel >(*it);
    }
}

void Scheduler::run(weak< Task::KernelTask > task, weak< const Kernel > kernel,
                    const minitl::array< weak< const IMemoryBuffer > >& parameters)
{
    /* TODO: set option to use Neon/AVX/SSE */
    weak< KernelObject > object
       = kernel->getResource(m_cpuLoaders[0]).getRefHandle< KernelObject >();
    be_assert(object, "kernel is not loaded");
    CPUKernelTask& taskBody = object->m_task->body;
    taskBody.sourceTask     = task;
    {
        minitl::array< weak< const IMemoryBuffer > > params = parameters;
        taskBody.params.swap(params);
    }
    object->m_task->schedule(m_scheduler);
}

weak< IMemoryHost > Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}  // namespace BugEngine::KernelScheduler::CPU
