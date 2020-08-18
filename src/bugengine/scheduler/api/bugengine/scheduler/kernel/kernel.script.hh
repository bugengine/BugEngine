/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
#define BE_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/resource/description.script.hh>
#include <bugengine/scheduler/kernel/code.script.hh>

namespace BugEngine { namespace KernelScheduler {

enum SchedulerType
{
    CPUType,
    GPUType
};

class be_api(SCHEDULER) Kernel : public Resource::Description
{
private:
    ref< const Code > m_kernelCode;
    const istring     m_name;

public:
    Kernel(ref< const Code > code, const istring& name);
    ~Kernel();

    istring name() const
    {
        return m_name;
    }
    weak< const Code > code() const
    {
        return m_kernelCode;
    }
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
