/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

SystemAllocator::SystemAllocator(u32 maximumBytes)
:   m_buffer(platformReserve(minitl::align(maximumBytes, platformPageSize())))
,   m_capacity(maximumBytes)
,   m_usage(0)
,   m_realUsage(0)
{
}

SystemAllocator::~SystemAllocator()
{
    be_assert(m_usage == 0, "Not all blocks reclaimed when system allocator was freed");
    platformFree(m_buffer, minitl::align(m_capacity, platformPageSize()));
}

void SystemAllocator::setUsage(u32 byteCount)
{
    m_usage = byteCount;
    u32 realUsage = minitl::align(byteCount, platformPageSize());
    if (realUsage < m_realUsage)
    {
        platformRelease(m_buffer, realUsage, m_realUsage);
    }
    else if (realUsage > m_realUsage)
    {
        platformCommit(m_buffer, m_realUsage, realUsage);
    }
    m_realUsage = realUsage;
}

}
