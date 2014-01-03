/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelloader.hh>
#include    <kernelobject.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

CPUKernelLoader::CPUKernelLoader()
    :   IKernelLoader()
{
}

CPUKernelLoader::~CPUKernelLoader()
{
}

void CPUKernelLoader::load(weak<const Resource::Description> kernelDescription, Resource::Resource& resource)
{
    be_info("loading CPU kernel %s"|be_checked_cast<const Kernel::KernelDescription>(kernelDescription)->name());
    resource.setRefHandle(ref<KernelObject>::create(Arena::task(), be_checked_cast<const Kernel::KernelDescription>(kernelDescription)->name()));
}

void CPUKernelLoader::reload(weak<const Resource::Description> /*oldKernelDescription*/,
                             weak<const Resource::Description> newKernelDescription,
                             Resource::Resource& resource)
{
    unload(resource);
    load(newKernelDescription, resource);
}

void CPUKernelLoader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}
