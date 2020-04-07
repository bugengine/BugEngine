/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <cuda/stdafx.h>
#include    <scheduler.hh>
#include    <codeloader.hh>
#include    <memoryhost.hh>

#include    <kernelobject.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <cuda_versions.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda
{

Scheduler::Scheduler(const Plugin::Context& context)
    :   IScheduler("Cuda", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_cudaLoaders(Arena::task(), s_cudaVersionCount)
    ,   m_memoryHost(scoped<MemoryHost>::create(Arena::task()))
{
    for (i32 i = 0; i < s_cudaVersionCount; ++i)
    {
        be_info("registering optimised Cuda kernel loader for %s" | s_cudaVersions[i]);
        m_cudaLoaders.push_back(ref<CodeLoader>::create(Arena::task(), istring(s_cudaVersions[i])));
        m_resourceManager->attach<Kernel>(m_cudaLoaders[i]);
    }
}

Scheduler::~Scheduler()
{
    for (minitl::vector< ref<CodeLoader> >::const_reverse_iterator it = m_cudaLoaders.rbegin();
         it != m_cudaLoaders.rend();
         ++it)
    {
        m_resourceManager->detach<Kernel>(*it);
    }
}

void Scheduler::run(weak<Task::KernelTask> task,
                    weak<const Kernel> kernel,
                    const minitl::array< weak<const IMemoryBuffer> >& parameters)
{
    weak<KernelObject> object = kernel->getResource(m_cudaLoaders[0]).getRefHandle<KernelObject>();
    be_assert(object, "kernel is not loaded");
    CudaKernelTask& taskBody = object->m_task->body;
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
