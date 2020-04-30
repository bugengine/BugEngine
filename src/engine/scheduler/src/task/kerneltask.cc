/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/task/kerneltask.hh>

namespace BugEngine { namespace Task {

KernelTask::KernelTask(istring name, KernelScheduler::SchedulerType type, color32 color,
                       Scheduler::Priority                                  priority,
                       weak< const BugEngine::KernelScheduler::Kernel >     kernel,
                       minitl::array< weak< KernelScheduler::IParameter > > parameters)
    : ITask(name, color, priority, Scheduler::WorkerThread)
    , m_kernel(kernel)
    , m_targetScheduler(KernelScheduler::IScheduler::findScheduler(type))
    , m_parameters(parameters)
{
}

KernelTask::~KernelTask()
{
}

void KernelTask::schedule(weak< Scheduler > scheduler)
{
    scheduler->queueKernel(this, m_parameters);
}

}}  // namespace BugEngine::Task
