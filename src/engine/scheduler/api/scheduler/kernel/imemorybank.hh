/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IMEMORYBANK_HH_
#define BE_SCHEDULER_KERNEL_IMEMORYBANK_HH_
/*****************************************************************************/
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine { namespace Kernel
{

class IMemoryProvider;

class be_api(SCHEDULER) IMemoryBank : public minitl::pointer
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