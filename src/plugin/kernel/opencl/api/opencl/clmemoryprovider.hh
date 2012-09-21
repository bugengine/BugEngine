/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
};

}

/*****************************************************************************/
#endif
