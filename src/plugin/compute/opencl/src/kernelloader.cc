/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <kernelloader.hh>

#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <codeloader.hh>
#include <codeobject.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

KernelLoader::KernelLoader(ref< CodeLoader > codeLoader) : IKernelLoader(codeLoader)
{
}

KernelLoader::~KernelLoader()
{
}

void KernelLoader::load(weak< const Resource::Description > kernelDescription,
                        Resource::Resource&                 resource)
{
    weak< const Kernel > kernel = be_checked_cast< const Kernel >(kernelDescription);
    be_info("loading OpenCL kernel %s" | kernel->name());
    weak< CodeObject > code
        = kernel->code()->getResource(m_codeLoader).getRefHandle< CodeObject >();
    resource.setRefHandle(ref< KernelObject >::create(Arena::task(), code, kernel->name()));
}

void KernelLoader::reload(weak< const Resource::Description > /*oldKernelDescription*/,
                          weak< const Resource::Description > newKernelDescription,
                          Resource::Resource&                 resource)
{
    unload(resource);
    load(newKernelDescription, resource);
}

void KernelLoader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
