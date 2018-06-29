/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/task/kerneltask.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace Task
{

KernelTask::KernelTask(istring name, color32 color, Scheduler::Priority priority,
                       weak<const BugEngine::KernelScheduler::Kernel> kernel,
                       minitl::array< weak<KernelScheduler::IParameter> > parameters)
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
