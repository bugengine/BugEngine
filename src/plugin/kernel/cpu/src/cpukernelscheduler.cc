/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelscheduler.hh>
#include    <cpukernelloader.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

CPUKernelScheduler::CPUKernelScheduler(const PluginContext& context)
    :   IKernelScheduler("CPU", context.scheduler, context.scheduler->memoryProvider())
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<CPUKernelLoader>::create(Arena::task()))
{
    m_resourceManager->attach<Kernel::Kernel>(m_loader);
}

CPUKernelScheduler::~CPUKernelScheduler()
{
    m_resourceManager->detach<Kernel::Kernel>(m_loader);
}

}
