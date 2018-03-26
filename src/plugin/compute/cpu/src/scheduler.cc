/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <scheduler.hh>
#include    <codeloader.hh>
#include    <memoryhost.hh>

#include    <kernelobject.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <kernel_optims.hh>

namespace BugEngine { namespace Kernel { namespace CPU
{

Scheduler::Scheduler(const Plugin::Context& context)
    :   IScheduler("CPU", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_cpuLoaders(Arena::task(), s_cpuVariantCount+1)
    ,   m_memoryHost(scoped<MemoryHost>::create(Arena::task()))
{
    for (i32 i = 0; i < s_cpuVariantCount; ++i)
    {
        be_info("registering optimised CPU kernel loader for %s" | s_cpuVariants[i]);
        m_cpuLoaders.push_back(ref<CodeLoader>::create(Arena::task(), inamespace(s_cpuVariants[i])));
        m_resourceManager->attach<Kernel>(m_cpuLoaders[i]);
    }
}

Scheduler::~Scheduler()
{
    for (minitl::vector< ref<CodeLoader> >::const_reverse_iterator it = m_cpuLoaders.rbegin();
         it != m_cpuLoaders.rend();
         ++it)
    {
        m_resourceManager->detach<Kernel>(*it);
    }
}

void Scheduler::run(weak<Task::KernelTask> task,
                    weak<const Kernel> kernel,
                    const minitl::array< weak<const IMemoryBuffer> >& parameters)
{
    /* TODO: set option to use Neon/AVX/SSE */
    weak<KernelObject> object = kernel->getResource(m_cpuLoaders[0]).getRefHandle<KernelObject>();
    be_assert(object, "kernel is not loaded");
    CPUKernelTask& taskBody = object->m_task->body;
    taskBody.sourceTask = task;
    {
        minitl::array< weak<const IMemoryBuffer> > params = parameters;
        taskBody.params.swap(params);
    }
    object->m_task->schedule(m_scheduler);
}

weak<IMemoryHost> Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}
