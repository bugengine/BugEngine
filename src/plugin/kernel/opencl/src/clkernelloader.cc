/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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

void OpenCLKernelLoader::load(weak<const Resource::Description> kernelDescription, Resource::Resource& /*resource*/)
{
    be_info("loading OpenCL kernel %s"|be_checked_cast<const Kernel::KernelDescription>(kernelDescription)->name());
    //resource.setRefHandle(ref<KernelObject>::create(Arena::task(), be_checked_cast<const Kernel>(kernelDescription)->name()));
}

void OpenCLKernelLoader::reload(weak<const Resource::Description> /*oldKernelDescription*/,
                                weak<const Resource::Description> newKernelDescription,
                                Resource::Resource& resource)
{
    unload(resource);
    load(newKernelDescription, resource);
}

void OpenCLKernelLoader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}
