/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <cuda/stdafx.h>
#include    <codeloader.hh>
#include    <kernelobject.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda
{

CodeLoader::CodeLoader(const istring& cudaVersion)
    :   ICodeLoader()
    ,   m_cudaVersion(cudaVersion)
{
}

CodeLoader::~CodeLoader()
{
}

void CodeLoader::load(weak<const Resource::Description> kernelDescription,
                      Resource::Resource& resource)
{
    be_info("loading Cuda kernel %s"
          | be_checked_cast<const Kernel>(kernelDescription)->name());
    inamespace name = be_checked_cast<const Kernel>(kernelDescription)->name() + inamespace("cuda");
    resource.setRefHandle(ref<KernelObject>::create(Arena::task(), name, m_cudaVersion));
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
