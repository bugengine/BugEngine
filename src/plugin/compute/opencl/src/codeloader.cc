/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <codeloader.hh>
#include <context.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

CodeLoader::CodeLoader(weak< const Context > context) : ICodeLoader(), m_context(context)
{
}

CodeLoader::~CodeLoader()
{
}

void CodeLoader::load(weak< const Resource::Description > kernelDescription,
                      Resource::Resource&                 resource)
{
    be_info("loading OpenCL kernel %s"
            | be_checked_cast< const Kernel >(kernelDescription)->name());
    inamespace name = be_checked_cast< const Kernel >(kernelDescription)->name() + inamespace("cl");
    resource.setRefHandle(ref< KernelObject >::create(Arena::task(), m_context, name));
}

void CodeLoader::reload(weak< const Resource::Description > /*oldKernelDescription*/,
                        weak< const Resource::Description > newKernelDescription,
                        Resource::Resource&                 resource)
{
    unload(resource);
    load(newKernelDescription, resource);
}

void CodeLoader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
