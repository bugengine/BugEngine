/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUMEMORYPROVIDER_HH_
#define BE_KERNEL_CPU_CPUMEMORYPROVIDER_HH_
/**************************************************************************************************/
#include    <scheduler/kernel/imemoryprovider.hh>
#include    <scheduler/kernel/istream.hh>

namespace BugEngine
{

class CPUMemoryProvider : public Kernel::IMemoryProvider
{
public:
    CPUMemoryProvider();
    ~CPUMemoryProvider();
protected:
    virtual Kernel::KernelParameter getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const override;
};

}

/**************************************************************************************************/
#endif
