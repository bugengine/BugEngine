/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/kernel/memoryhost.hh>


namespace BugEngine { namespace World
{

MemoryHost::MemoryHost(const SystemAllocator& pageAllocator)
    :   Kernel::IMemoryHost("World")
    ,   m_allocator(pageAllocator)
{
    be_forceuse(m_allocator);
}

MemoryHost::~MemoryHost()
{
}

}}