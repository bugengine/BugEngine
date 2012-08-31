/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUKERNELLOADER_HH_
#define BE_KERNEL_CPU_CPUKERNELLOADER_HH_
/*****************************************************************************/
#include    <scheduler/kernel/ikernelloader.hh>


namespace BugEngine
{

class CPUKernelLoader : public Kernel::IKernelLoader
{
public:
    CPUKernelLoader();
    ~CPUKernelLoader();

    virtual void load(weak<const Resource::Description> kernelDescription, Resource::Resource& resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}


/*****************************************************************************/
#endif
