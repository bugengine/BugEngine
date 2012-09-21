/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_CPUMEMORYBANK_HH_
#define BE_SCHEDULER_KERNEL_CPUMEMORYBANK_HH_
/*****************************************************************************/
#include    <scheduler/kernel/imemorybank.hh>

namespace BugEngine { namespace Kernel
{

class CPUMemoryProvider;

class CPUMemoryBank : public IMemoryBank
{
public:
    CPUMemoryBank(weak<const CPUMemoryProvider> provider);
    ~CPUMemoryBank();
};

}}

/*****************************************************************************/
#endif
