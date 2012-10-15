/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelscheduler.hh>
#include    <cpukernelloader.hh>
#include    <cpumemoryprovider.hh>

#include    <kernelobject.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

struct CPUKernelTask
{
    minitl::array<Kernel::KernelParameter> params;

    CPUKernelTask(const minitl::array<Kernel::KernelParameter>& params)
        :   params(Arena::task(), params.begin(), params.end())
    {
    }
};

CPUKernelScheduler::CPUKernelScheduler(const Plugin::Context& context)
    :   IKernelScheduler("CPU", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<CPUKernelLoader>::create(Arena::task()))
    ,   m_cpuMemoryProvider(scoped<CPUMemoryProvider>::create(Arena::task()))
{
    m_resourceManager->attach<Kernel::KernelDescription>(m_loader);
}

CPUKernelScheduler::~CPUKernelScheduler()
{
    m_resourceManager->detach<Kernel::KernelDescription>(m_loader);
}

void CPUKernelScheduler::run(weak<const Task::ITask> task, weak<const Kernel::KernelDescription> kernel, const minitl::array<Kernel::KernelParameter>& parameters)
{
    weak<KernelObject> object = kernel->getResource(m_loader).getRefHandle<KernelObject>();
    be_assert(object, "kernel is not loaded");
    CPUKernelTask t(parameters);
    task->completed(m_scheduler);
}

weak<Kernel::IMemoryProvider> CPUKernelScheduler::memoryProvider() const
{
    return m_cpuMemoryProvider;
}

}
