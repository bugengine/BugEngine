/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
/*****************************************************************************/

namespace BugEngine
{

class Scheduler;

namespace Kernel
{

class be_api(SYSTEM) IKernelScheduler : public minitl::pointer
{
private:
    istring const           m_name;
    weak<Scheduler> const   m_scheduler;
public:
    IKernelScheduler(const istring& name, weak<Scheduler> scheduler);
    ~IKernelScheduler();
};

}}

/*****************************************************************************/
#endif
