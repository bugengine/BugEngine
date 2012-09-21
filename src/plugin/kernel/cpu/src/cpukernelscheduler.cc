/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelscheduler.hh>
#include    <cpukernelloader.hh>

#include "scheduler/kernel/istream.hh"
#include    <kernelobject.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/kernel/cpumemoryprovider.hh>

namespace BugEngine
{

struct CPUKernelTask
{
    minitl::array<KernelObjectParameter> params;

    CPUKernelTask(u32 parameterCount) : params(Arena::task(), parameterCount)   {}
};

CPUKernelScheduler::CPUKernelScheduler(const Plugin::Context& context)
    :   IKernelScheduler("CPU", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<CPUKernelLoader>::create(Arena::task()))
    ,   m_cpuMemoryProvider(scoped<Kernel::CPUMemoryProvider>::create(Arena::task()))
{
    m_resourceManager->attach<Kernel::KernelDescription>(m_loader);
}

CPUKernelScheduler::~CPUKernelScheduler()
{
    m_resourceManager->detach<Kernel::KernelDescription>(m_loader);
}

void CPUKernelScheduler::run(weak<const Kernel::KernelDescription> kernel, const minitl::array< weak<Kernel::IStream> >& parameters)
{
    weak<KernelObject> object = kernel->getResource(m_loader).getRefHandle<KernelObject>();
    be_assert(object, "kernel is not loaded");
    CPUKernelTask task(parameters.size());
    for (u32 i = 0; i < parameters.size(); ++i)
    {
        //task.params[i] = parameters[i]->getBank(m_memoryProvider);
    }
}

weak<Kernel::IMemoryProvider> CPUKernelScheduler::memoryProvider() const
{
    return m_cpuMemoryProvider;
}

}
