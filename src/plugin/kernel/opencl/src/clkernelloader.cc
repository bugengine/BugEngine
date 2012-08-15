/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <clkernelloader.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

OpenCLKernelLoader::OpenCLKernelLoader()
    :   IKernelLoader()
{
}

OpenCLKernelLoader::~OpenCLKernelLoader()
{
}

ResourceHandle OpenCLKernelLoader::load(weak<const Resource> resource)
{
    be_info("loading OpenCL kernel %s"|be_checked_cast<const Kernel::Kernel>(resource)->name());
    ResourceHandle handle;
    //handle.handle = ref<KernelObject>::create(Arena::task(), be_checked_cast<const Kernel>(resource)->name());
    return handle;
}

void OpenCLKernelLoader::unload(const ResourceHandle& resource)
{
    be_forceuse(resource);
}

}
