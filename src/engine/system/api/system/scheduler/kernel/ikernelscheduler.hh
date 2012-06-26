/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
/*****************************************************************************/

namespace BugEngine
{

class Scheduler;
class be_api(SYSTEM) IKernelScheduler : public minitl::pointer
{
private:
    istring const           m_name;
public:
    IKernelScheduler(const istring& name);
    ~IKernelScheduler();
};

}

/*****************************************************************************/
#endif
