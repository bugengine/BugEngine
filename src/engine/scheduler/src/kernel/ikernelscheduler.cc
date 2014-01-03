/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/ikernelscheduler.hh>
#include    <scheduler/scheduler.hh>

namespace BugEngine { namespace Kernel
{

IKernelScheduler::IKernelScheduler(const istring& name, weak<Scheduler> scheduler)
    :   m_name(name)
    ,   m_scheduler(scheduler)
{
    m_scheduler->addKernelScheduler(this);
}

IKernelScheduler::~IKernelScheduler()
{
    m_scheduler->removeKernelScheduler(this);
}

}}
