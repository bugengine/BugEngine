/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_OPENCL_CLKERNELLOADER_HH_
#define BE_KERNEL_OPENCL_CLKERNELLOADER_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/ikernelloader.hh>


namespace BugEngine
{

class OpenCLKernelLoader : public Kernel::IKernelLoader
{
public:
    OpenCLKernelLoader();
    ~OpenCLKernelLoader();

    virtual ResourceHandle load(weak<const Resource> resource) override;
    virtual void unload(const ResourceHandle& resource) override;
};

}


/*****************************************************************************/
#endif
