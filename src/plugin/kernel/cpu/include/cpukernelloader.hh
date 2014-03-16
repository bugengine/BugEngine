/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUKERNELLOADER_HH_
#define BE_KERNEL_CPU_CPUKERNELLOADER_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <scheduler/kernel/ikernelloader.hh>


namespace BugEngine
{

class CPUKernelLoader : public Kernel::IKernelLoader
{
public:
    CPUKernelLoader();
    ~CPUKernelLoader();

    virtual void load(weak<const Resource::Description> kernelDescription, Resource::Resource& resource) override;
    virtual void reload(weak<const Resource::Description> oldKernelDescription,
                        weak<const Resource::Description> newKernelDescription,
                         Resource::Resource& resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}


/**************************************************************************************************/
#endif
