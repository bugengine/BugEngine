/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASK_KERNELTASK_HH_
#define BE_SCHEDULER_TASK_KERNELTASK_HH_
/*****************************************************************************/
#include    <scheduler/task/itask.hh>

namespace BugEngine { namespace Kernel
{
class Kernel;
}}

namespace BugEngine { namespace Task
{

class be_api(SCHEDULER) KernelTask : public ITask
{
    BE_NOCOPY(KernelTask);
private:
    weak<const BugEngine::Kernel::Kernel> const m_kernel;
public:
    KernelTask(istring name, color32 color, Scheduler::Priority priority, weak<const BugEngine::Kernel::Kernel> kernel);
    ~KernelTask();

    virtual void schedule(weak<Scheduler> scheduler) const override;
};

}}

/*****************************************************************************/
#endif
