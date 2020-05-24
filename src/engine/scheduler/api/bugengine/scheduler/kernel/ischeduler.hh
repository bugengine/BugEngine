/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_ISCHEDULER_HH_
#define BE_SCHEDULER_KERNEL_ISCHEDULER_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/imemorybuffer.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/task/kerneltask.hh>

namespace BugEngine {
class Scheduler;
}

namespace BugEngine { namespace KernelScheduler {

class IMemoryHost;

class be_api(SCHEDULER) IKernelTaskItem
{
    friend class BugEngine::Scheduler;

protected:
    weak< Task::KernelTask >                     m_owner;
    weak< const Kernel >                         m_kernel;
    minitl::array< weak< const IMemoryBuffer > > m_parameters;

protected:
    IKernelTaskItem(weak< Task::KernelTask > ownerTask, weak< const Kernel > kernel,
                    u32 parameterCount);
    virtual ~IKernelTaskItem();

public:
    weak< Task::KernelTask > owner() const
    {
        return m_owner;
    }
    weak< const Kernel > kernel() const
    {
        return m_kernel;
    }
    const minitl::array< weak< const IMemoryBuffer > >& parameters() const
    {
        return m_parameters;
    }
};

class be_api(SCHEDULER) IScheduler : public minitl::refcountable
{
protected:
    const weak< Scheduler > m_scheduler;
    const istring           m_name;
    const SchedulerType     m_type;

protected:
    IScheduler(istring name, weak< Scheduler > scheduler, SchedulerType type);
    virtual ~IScheduler();

public:
    virtual IKernelTaskItem* allocateItem(weak< Task::KernelTask > task,
                                          weak< const Kernel > kernel, u32 parametercount)
        = 0;
    virtual void                run(IKernelTaskItem * item) = 0;
    virtual weak< IMemoryHost > memoryHost() const          = 0;

    static weak< IScheduler > findScheduler(SchedulerType preferredType);
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
