/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clkernelloader.hh>
#include    <system/scheduler/kernel/kernel.script.hh>

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
