/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUMEMORYBANK_HH_
#define BE_KERNEL_CPU_CPUMEMORYBANK_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <scheduler/kernel/imemorybank.hh>

namespace BugEngine
{

class CPUMemoryProvider;

class CPUMemoryBank : public Kernel::IMemoryBank
{
public:
    CPUMemoryBank(weak<const CPUMemoryProvider> provider);
    ~CPUMemoryBank();
};

}

/**************************************************************************************************/
#endif
