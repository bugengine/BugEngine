/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelloader.hh>
#include    <kernelobject.hh>
#include    <system/scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

CpuKernelLoader::CpuKernelLoader()
    :   IKernelLoader()
{
}

CpuKernelLoader::~CpuKernelLoader()
{
}

ResourceHandle CpuKernelLoader::load(weak<const Resource> resource)
{
    be_info("loading CPU kernel %s"|be_checked_cast<const Kernel::Kernel>(resource)->name());
    ResourceHandle handle;
    handle.handle = ref<KernelObject>::create(Arena::task(), be_checked_cast<const Kernel::Kernel>(resource)->name());
    return handle;
}

void CpuKernelLoader::unload(const ResourceHandle& resource)
{
    be_forceuse(resource);
}

}
