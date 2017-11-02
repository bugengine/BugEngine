/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/ischeduler.hh>
#include    <scheduler/scheduler.hh>


namespace BugEngine { namespace Kernel
{

IScheduler::IScheduler(istring name, weak<Scheduler> scheduler)
    :   m_name(name)
    ,   m_scheduler(scheduler)
{
    m_scheduler->addKernelScheduler(this);
}

IScheduler::~IScheduler()
{
    m_scheduler->removeKernelScheduler(this);
}

}}
