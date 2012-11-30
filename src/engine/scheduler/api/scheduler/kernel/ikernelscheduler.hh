/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
#define BE_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
/*****************************************************************************/
#include    <minitl/array.hh>

namespace BugEngine
{

class Scheduler;

namespace Task
{
class ITask;
}

namespace Kernel
{

class IMemoryProvider;
class IStream;
class KernelDescription;

struct KernelParameter;

class be_api(SCHEDULER) IKernelScheduler : public minitl::pointer
{
    friend class ::BugEngine::Scheduler;
protected:
    istring const               m_name;
    weak<Scheduler> const       m_scheduler;
public:
    IKernelScheduler(const istring& name, weak<Scheduler> scheduler);
    ~IKernelScheduler();

    virtual weak<IMemoryProvider> memoryProvider() const = 0;
    virtual void run(weak<const Task::ITask> task, weak<const Kernel::KernelDescription> kernel, const minitl::array<KernelParameter>& parameters) = 0;
};

}}

/*****************************************************************************/
#endif
