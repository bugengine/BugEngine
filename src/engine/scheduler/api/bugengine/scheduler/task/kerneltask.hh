/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASK_KERNELTASK_HH_
#define BE_SCHEDULER_TASK_KERNELTASK_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>
#include <bugengine/scheduler/task/itask.hh>

namespace BugEngine { namespace KernelScheduler {
class Kernel;
class IScheduler;
}}  // namespace BugEngine::KernelScheduler

namespace BugEngine {
class Scheduler;
}

namespace BugEngine { namespace Task {

class be_api(SCHEDULER) KernelTask : public ITask
{
    friend class ::BugEngine::Scheduler;
    BE_NOCOPY(KernelTask);

private:
    weak< const KernelScheduler::Kernel > const                m_kernel;
    weak< KernelScheduler::IScheduler >                        m_targetScheduler;
    minitl::array< weak< KernelScheduler::IParameter > > const m_parameters;
    u32                                                        m_subTaskCount;

public:
    KernelTask(istring name, KernelScheduler::SchedulerType type, color32 color,
               Scheduler::Priority                                  priority,
               weak< const BugEngine::KernelScheduler::Kernel >     kernel,
               minitl::array< weak< KernelScheduler::IParameter > > parameters);
    ~KernelTask();

    virtual void schedule(weak< Scheduler > scheduler) override;
};

}}  // namespace BugEngine::Task

/**************************************************************************************************/
#endif
