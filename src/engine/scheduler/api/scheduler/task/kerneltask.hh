/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASK_KERNELTASK_HH_
#define BE_SCHEDULER_TASK_KERNELTASK_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/parameters/iparameter.script.hh>


namespace BugEngine { namespace Kernel
{
class Kernel;
}}

namespace BugEngine
{
class Scheduler;
}

namespace BugEngine { namespace Task
{

class be_api(SCHEDULER) KernelTask : public ITask
{
    friend class ::BugEngine::Scheduler;
    BE_NOCOPY(KernelTask)
private:
    weak<const Kernel::Kernel> const                m_kernel;
    minitl::array< weak<Kernel::IParameter> > const m_parameters;
public:
    KernelTask(istring name, color32 color, Scheduler::Priority priority,
               weak<const BugEngine::Kernel::Kernel> kernel,
               minitl::array< weak<Kernel::IParameter> > parameters);
    ~KernelTask();

    virtual void schedule(weak<Scheduler> scheduler) override;
};

}}

/**************************************************************************************************/
#endif
