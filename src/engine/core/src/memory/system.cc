/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

SystemAllocator::SystemAllocator(BlockSize size, u32 initialCount)
    :   m_head(0)
    ,   m_capacity(i_u32::Zero)
    ,   m_used(i_u32::Zero)
    ,   m_blockSize(size)
{
    grow(initialCount);
}

SystemAllocator::~SystemAllocator()
{
    be_assert(m_used == 0, "Not all blocks reclaimed when system allocator was freed");
    Block* head = m_head;
    while (head)
    {
        byte* buffer = reinterpret_cast<byte*>(head);
        head = head->next;
        if (blockSize() < platformPageSize())
        {
            if ((uintptr_t)buffer % platformPageSize() == 0)
            {
                platformFree(buffer, platformPageSize());
            }
        }
        else
        {
            platformFree(buffer, blockSize());
        }
    }
}

void* SystemAllocator::allocate()
{
    ++ m_used;
    itaggedptr<Block>::ticket_t ticket;
    Block* result;
    do
    {
        ticket = m_head.getTicket();
        result = m_head;
        if (!result)
        {
            ScopedMutexLock lock(m_allocLock);
            if (!m_head)
            {
                grow(m_capacity);
            }
            ticket = m_head.getTicket();
            result = m_head;
        }
    } while (!m_head.setConditional(result->next, ticket));
    return result;
}

void  SystemAllocator::free(void* memory)
{
    -- m_used;
    itaggedptr<Block>::ticket_t ticket;
    Block* block;
    do
    {
        ticket = m_head.getTicket();
        block = reinterpret_cast<Block*>(memory);
        block->next = m_head;
    } while (!m_head.setConditional(block, ticket));
}

u32 SystemAllocator::blockSize() const
{
    return 1 << (((u32)m_blockSize * 2) + 12);
}

void SystemAllocator::grow(u32 extraCapacity)
{
    Block* head = 0;
    const u32 pageSize = platformPageSize();
    if (blockSize() > pageSize)
    {
        for (u32 i = 0; i < extraCapacity; ++i)
        {
            Block* block = (Block*)platformReserve(blockSize());
            platformCommit((byte*)block, 0, blockSize());
            block->next = head;
            head = block;
        }
    }
    else
    {
        be_assert(pageSize % blockSize() == 0, "Page size should be a multiple of block size");
        u32 blocksPerPage = pageSize / blockSize();
        u32 pageCount = extraCapacity / blocksPerPage;
        for (u32 i = 0; i < pageCount; ++i)
        {
            Block* block = (Block*)platformReserve(pageSize);
            platformCommit((byte*)block, 0, pageSize);
            for (u32 j = 0; j < blocksPerPage; ++j)
            {
                block->next = head;
                head = block;
                block = (Block*)((u8*)block + blockSize());
            }
        }
    }
    m_head.setConditional(head, m_head.getTicket());
    m_capacity += extraCapacity;
}

}
