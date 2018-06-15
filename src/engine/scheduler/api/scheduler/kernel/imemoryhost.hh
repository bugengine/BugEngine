/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IMEMORYHOST_HH_
#define BE_SCHEDULER_KERNEL_IMEMORYHOST_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <resource/description.script.hh>

namespace BugEngine { namespace KernelScheduler
{

class be_api(SCHEDULER) IMemoryHost : public minitl::refcountable
{
protected:
    const istring   m_name;
protected:
    IMemoryHost(const istring& name);
    ~IMemoryHost();
};

}}

/**************************************************************************************************/
#endif
