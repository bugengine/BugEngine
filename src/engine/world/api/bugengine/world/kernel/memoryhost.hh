/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_KERNEL_MEMORYPROVIDER_HH_
#define BE_WORLD_KERNEL_MEMORYPROVIDER_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/core/memory/allocators/system.hh>
#include <bugengine/scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace World {

class be_api(WORLD) MemoryHost : public KernelScheduler::IMemoryHost
{
private:
    const SystemAllocator& m_allocator;

public:
    explicit MemoryHost(const SystemAllocator& pageAllocator);
    ~MemoryHost();

    void release(weak< KernelScheduler::IMemoryBuffer > buffer);
};

}}  // namespace BugEngine::World

/**************************************************************************************************/
#endif
