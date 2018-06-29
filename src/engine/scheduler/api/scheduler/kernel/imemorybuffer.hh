/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IMEMORYBUFFER_HH_
#define BE_SCHEDULER_KERNEL_IMEMORYBUFFER_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>


namespace BugEngine { namespace KernelScheduler
{

class be_api(SCHEDULER) IMemoryBuffer : public minitl::refcountable
{
protected:
    weak<const IMemoryHost> const   m_host;
protected:
    IMemoryBuffer(weak<const IMemoryHost> host);
    ~IMemoryBuffer();

public:
    weak<const IMemoryHost> getHost() const { return m_host; }
};

}}

/**************************************************************************************************/
#endif
