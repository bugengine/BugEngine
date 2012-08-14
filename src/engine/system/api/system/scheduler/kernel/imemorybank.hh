/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_IMEMORYBANK_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_IMEMORYBANK_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/imemoryprovider.hh>

namespace BugEngine { namespace Kernel
{

class IMemoryProvider;

class be_api(SYSTEM) IMemoryBank : public minitl::pointer
{
private:
    weak<const IMemoryProvider> const   m_owner;
protected:
    IMemoryBank(weak<const IMemoryProvider> owner);
public:
    weak<const IMemoryProvider> provider() const    { return m_owner; }
};

}}

/*****************************************************************************/
#endif
