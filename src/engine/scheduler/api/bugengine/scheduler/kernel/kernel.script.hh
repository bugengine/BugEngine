/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
#define BE_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/resource/description.script.hh>

namespace BugEngine { namespace KernelScheduler {

enum SchedulerType
{
    CPUType,
    GPUType
};

class be_api(SCHEDULER) Kernel
    : public Resource::Description
    , public minitl::intrusive_list< const Kernel, 1 >::item
{
public:
    typedef minitl::intrusive_list< const Kernel, 1 > KernelList;

protected:
    const inamespace m_name;

protected:
    Kernel(const inamespace& name);
    ~Kernel();

public:
    inamespace name() const
    {
        return m_name;
    }
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
