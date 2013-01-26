/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelscheduler.hh>
#include    <cpukernelloader.hh>
#include    <cpumemoryprovider.hh>

#include    <kernelobject.hh>
#include    <scheduler/kernel/istream.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

CPUKernelScheduler::CPUKernelScheduler(const Plugin::Context& context)
    :   IKernelScheduler("CPU", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<CPUKernelLoader>::create(Arena::task()))
    ,   m_cpuMemoryProvider(scoped<CPUMemoryProvider>::create(Arena::task()))
{
    m_resourceManager->attach<Kernel::KernelDescription>(weak<Resource::ILoader>(m_loader));
}

CPUKernelScheduler::~CPUKernelScheduler()
{
    m_resourceManager->detach<Kernel::KernelDescription>(weak<const Resource::ILoader>(m_loader));
}

void CPUKernelScheduler::run(weak<const Task::ITask> task, weak<const Kernel::KernelDescription> kernel, const minitl::array<Kernel::KernelParameter>& parameters)
{
    weak<KernelObject> object = kernel->getResource(m_loader).getRefHandle<KernelObject>();
    be_assert(object, "kernel is not loaded");
    CPUKernelTask& taskBody = object->m_task->body;
    taskBody.sourceTask = task;
    for (u32 i = 0; i < parameters.size(); ++i)
    {
        taskBody.params[i] = parameters[i];
    }
    object->m_task->schedule(m_scheduler);
}

weak<Kernel::IMemoryProvider> CPUKernelScheduler::memoryProvider() const
{
    return m_cpuMemoryProvider;
}

}
