/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

#ifdef BE_ENABLE_MEMORY_TRACKING
static const u32 s_debugExtraSpace = 2;
#else
static const u32 s_debugExtraSpace = 1;
#endif

SystemAllocator::SystemAllocator(BlockSize size, u32 blockCount)
:   m_pool()
,   m_buffer(platformReserve(s_debugExtraSpace*be_checked_numcast<u32>((1ll<<((u64)size))*blockCount)))
,   m_blockSize(size)
,   m_blockCount(0)
,   m_maximumBlockCount(blockCount)
,   m_usedBlockCount(0)
{
}

SystemAllocator::~SystemAllocator()
{
    be_assert(m_usedBlockCount == 0, "Not all blocks reclaimed when system allocator was freed");
    platformFree(m_buffer, be_checked_numcast<u32>((1ll<<((u64)m_blockSize))*m_maximumBlockCount));
}

void* SystemAllocator::blockAlloc()
{
    void* result = 0;
    while(!result)
    {
        minitl::itaggedptr<void>::ticket_t ticket;
        do
        {
            ticket = m_pool.getTicket();
            result = m_pool;
        } while(result && !m_pool.setConditional(*(void**)result, ticket));
        if (!result)
        {
            grow();
        }
    }
    ++m_usedBlockCount;
    return result;
}

void SystemAllocator::blockFree(void* block)
{
    minitl::itaggedptr<void>::ticket_t ticket;
    do
    {
        ticket = m_pool.getTicket();
        *(void**)block = m_pool;
    } while(!m_pool.setConditional(block, ticket));
    --m_usedBlockCount;
}

void SystemAllocator::grow()
{
    
    static const u32 s_growSize = 4;
    u32 growSize = minitl::min(s_growSize, m_maximumBlockCount-m_blockCount);
    for (u32 i = 0; i < growSize; ++i)
    {
        void* newBlock = platformCommit(
                m_buffer,
                (s_debugExtraSpace*m_blockCount + 0)*be_checked_numcast<u32>((1ll<<((u64)m_blockSize))),
                (s_debugExtraSpace*m_blockCount + 1)*be_checked_numcast<u32>((1ll<<((u64)m_blockSize)))
            );
        blockFree(newBlock);
        m_blockCount++;
    }
}

}
