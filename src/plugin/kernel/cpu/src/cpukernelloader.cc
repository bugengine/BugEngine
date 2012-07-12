/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelloader.hh>
#include    <kernelobject.hh>
#include    <system/scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

CPUKernelLoader::CPUKernelLoader()
    :   IKernelLoader()
{
}

CPUKernelLoader::~CPUKernelLoader()
{
}

ResourceHandle CPUKernelLoader::load(weak<const Resource> resource)
{
    be_info("loading CPU kernel %s"|be_checked_cast<const Kernel::Kernel>(resource)->name());
    ResourceHandle handle;
    handle.handle = ref<KernelObject>::create(Arena::task(), be_checked_cast<const Kernel::Kernel>(resource)->name());
    return handle;
}

void CPUKernelLoader::unload(const ResourceHandle& resource)
{
    be_forceuse(resource);
}

}
