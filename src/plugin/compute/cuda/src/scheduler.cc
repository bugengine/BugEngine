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
    :   IScheduler("Cuda", context.scheduler, GPUType)
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
    /* todo */
    be_forceuse(task);
    be_forceuse(kernel);
    be_forceuse(parameters);
}

weak<IMemoryHost> Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}
