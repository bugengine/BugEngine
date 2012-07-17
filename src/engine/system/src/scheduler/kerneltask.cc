/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/task/kerneltask.hh>
#include    <system/scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace Task
{

KernelTask::KernelTask(istring name, color32 color, Scheduler::Priority priority, weak<const BugEngine::Kernel::Kernel> kernel)
:   ITask(name, color, priority, Scheduler::DontCare)
,   m_kernel(kernel)
{
}

KernelTask::~KernelTask()
{
}

void KernelTask::schedule(weak<Scheduler> scheduler) const
{
    scheduler->queueKernel();
}

}}
