/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <codeloader.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL
{

CodeLoader::CodeLoader()
    :   ICodeLoader()
{
}

CodeLoader::~CodeLoader()
{
}

void CodeLoader::load(weak<const Resource::Description> kernelDescription, Resource::Resource& /*resource*/)
{
    be_info("loading OpenCL kernel %s"|be_checked_cast<const Kernel>(kernelDescription)->name());
    //resource.setRefHandle(ref<KernelObject>::create(Arena::task(), be_checked_cast<const Kernel>(kernelDescription)->name()));
}

void CodeLoader::reload(weak<const Resource::Description> /*oldKernelDescription*/,
                        weak<const Resource::Description> newKernelDescription,
                        Resource::Resource& resource)
{
    unload(resource);
    load(newKernelDescription, resource);
}

void CodeLoader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}}}
