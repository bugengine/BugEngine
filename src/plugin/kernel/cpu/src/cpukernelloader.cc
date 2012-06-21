/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelloader.hh>
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
    be_forceuse(resource);
    be_info("loading CPU kernel %s"|be_checked_cast<const Kernel>(resource)->name());
    return ResourceHandle();
}

void CpuKernelLoader::unload(const ResourceHandle& resource)
{
    be_forceuse(resource);
}

void CpuKernelLoader::onTicketLoaded(weak<const Resource> resource, const Allocator::Block<u8>& buffer)
{
    be_forceuse(resource);
    be_forceuse(buffer);
}

}
