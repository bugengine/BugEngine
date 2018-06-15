/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_ISCHEDULER_HH_
#define BE_SCHEDULER_KERNEL_ISCHEDULER_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/task/kerneltask.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/kernel/imemorybuffer.hh>

namespace BugEngine
{
class Scheduler;
}

namespace BugEngine { namespace KernelScheduler
{

class IMemoryHost;

class be_api(SCHEDULER) IScheduler : public minitl::refcountable
{
public:
protected:
    const istring           m_name;
    const weak<Scheduler>   m_scheduler;

protected:
    IScheduler(istring name, weak<Scheduler> scheduler);
    virtual ~IScheduler();

public:
    virtual void run(weak<Task::KernelTask> task,
                     weak<const Kernel> kernel,
                     const minitl::array< weak<const IMemoryBuffer> >& parameters) = 0;
    virtual weak<IMemoryHost> memoryHost() const = 0;
};

}}

/**************************************************************************************************/
#endif
