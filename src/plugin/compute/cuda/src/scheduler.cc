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

namespace BugEngine { namespace KernelScheduler { namespace Cuda
{

Scheduler::Scheduler(const Plugin::Context& context)
    :   IScheduler("Cuda", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_cudaLoader(ref<CodeLoader>::create(Arena::task()))
    ,   m_memoryHost(scoped<MemoryHost>::create(Arena::task()))
{
    m_resourceManager->attach<Kernel>(m_cudaLoader);
}

Scheduler::~Scheduler()
{
    m_resourceManager->detach<Kernel>(m_cudaLoader);
}

void Scheduler::run(weak<Task::KernelTask> task,
                    weak<const Kernel> kernel,
                    const minitl::array< weak<const IMemoryBuffer> >& parameters)
{
    weak<KernelObject> object = kernel->getResource(m_cudaLoader).getRefHandle<KernelObject>();
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
