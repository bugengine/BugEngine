/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASK_KERNELTASK_HH_
#define BE_SCHEDULER_TASK_KERNELTASK_HH_
/*****************************************************************************/
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/istream.hh>

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
    weak<const Kernel::KernelDescription> const         m_kernel;
    minitl::array< weak<const Kernel::IStream> > const  m_parameters;
public:
    KernelTask(istring name, color32 color, Scheduler::Priority priority,
               weak<const BugEngine::Kernel::KernelDescription> kernel,
               minitl::array< weak<const Kernel::IStream> > parameters);
    ~KernelTask();

    virtual void schedule(weak<Scheduler> scheduler) override;
};

}}

/*****************************************************************************/
#endif
