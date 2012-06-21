/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUKERNELLOADER_HH_
#define BE_KERNEL_CPU_CPUKERNELLOADER_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/ikernelloader.hh>


namespace BugEngine
{

class CpuKernelLoader : public IKernelLoader
{
public:
    CpuKernelLoader();
    ~CpuKernelLoader();

    virtual ResourceHandle load(weak<const Resource> resource) override;
    virtual void unload(const ResourceHandle& resource) override;

    virtual void onTicketLoaded(weak<const Resource> resource, const Allocator::Block<u8>& buffer) override;
};

}


/*****************************************************************************/
#endif
