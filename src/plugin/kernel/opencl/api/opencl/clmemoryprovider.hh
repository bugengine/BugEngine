/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_OPENCL_CLMEMORYPROVIDER_HH_
#define BE_KERNEL_OPENCL_CLMEMORYPROVIDER_HH_
/*****************************************************************************/
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine
{

class OpenCLMemoryProvider : public Kernel::IMemoryProvider
{
public:
    OpenCLMemoryProvider();
    ~OpenCLMemoryProvider();
protected:
    virtual Kernel::KernelParameter getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const override;
};

}

/*****************************************************************************/
#endif
