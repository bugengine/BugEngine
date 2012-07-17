/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/kernel/ikernelscheduler.hh>
#include    <system/scheduler/scheduler.hh>

namespace BugEngine { namespace Kernel
{

IKernelScheduler::IKernelScheduler(const istring& name, weak<Scheduler> scheduler, weak<IMemoryProvider> provider)
    :   m_name(name)
    ,   m_scheduler(scheduler)
    ,   m_memoryProvider(provider)
{
    m_scheduler->addKernelScheduler(this);
}

IKernelScheduler::~IKernelScheduler()
{
    m_scheduler->removeKernelScheduler(this);
}

}}
