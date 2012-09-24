/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASK_KERNELTASK_HH_
#define BE_SCHEDULER_TASK_KERNELTASK_HH_
/*****************************************************************************/
#include    <scheduler/task/itask.hh>

namespace BugEngine { namespace Kernel
{
class KernelDescription;
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
    BE_NOCOPY(KernelTask);
private:
    weak<const BugEngine::Kernel::KernelDescription> const m_kernel;
public:
    KernelTask(istring name, color32 color, Scheduler::Priority priority, weak<const BugEngine::Kernel::KernelDescription> kernel);
    ~KernelTask();

    virtual void schedule(weak<Scheduler> scheduler) const override;
};

}}

/*****************************************************************************/
#endif
