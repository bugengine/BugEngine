/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_KERNEL_MEMORYPROVIDER_HH_
#define BE_WORLD_KERNEL_MEMORYPROVIDER_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>
#include    <core/memory/allocators/system.hh>


namespace BugEngine { namespace World
{

class be_api(WORLD) MemoryHost : public Kernel::IMemoryHost
{
private:
    const SystemAllocator&  m_allocator;
public:
    explicit MemoryHost(const SystemAllocator& pageAllocator);
    ~MemoryHost();
};

}}


/**************************************************************************************************/
#endif
