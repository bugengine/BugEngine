/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/task/kerneltask.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace Task
{

KernelTask::KernelTask(istring name, color32 color, Scheduler::Priority priority,
                       weak<const BugEngine::Kernel::KernelDescription> kernel,
                       minitl::array< weak<const Kernel::IStream> > parameters)
:   ITask(name, color, priority, Scheduler::DontCare)
,   m_kernel(kernel)
,   m_parameters(parameters)
{
}

KernelTask::~KernelTask()
{
}

void KernelTask::schedule(weak<Scheduler> scheduler)
{
    scheduler->queueKernel(this, m_parameters);
}

}}
