/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelscheduler.hh>
#include    <cpukernelloader.hh>
#include    <system/resource/resourcemanager.hh>
#include    <system/scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

CpuKernelScheduler::CpuKernelScheduler(const PluginContext& context)
    :   IKernelScheduler("CPU")
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<CpuKernelLoader>::create(Arena::task()))
{
    m_resourceManager->attach<Kernel>(m_loader);
}

CpuKernelScheduler::~CpuKernelScheduler()
{
    m_resourceManager->detach<Kernel>(m_loader);
}

}
