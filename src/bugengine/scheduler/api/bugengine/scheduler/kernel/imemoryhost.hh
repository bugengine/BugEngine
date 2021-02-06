/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IMEMORYHOST_HH_
#define BE_SCHEDULER_KERNEL_IMEMORYHOST_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/resource/description.script.hh>

namespace BugEngine { namespace KernelScheduler {

class IMemoryBuffer;

class be_api(SCHEDULER) IMemoryHost : public minitl::refcountable
{
    friend class IMemoryBuffer;

protected:
    const istring m_name;

protected:
    IMemoryHost(const istring& name);
    ~IMemoryHost();

    virtual void release(weak< IMemoryBuffer > buffer) = 0;
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
