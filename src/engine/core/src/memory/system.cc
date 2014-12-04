/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

SystemAllocator::SystemAllocator(BlockSize size, u32 initialCount)
    :   m_head(0)
    ,   m_capacity(i_u32::create(initialCount))
    ,   m_used(i_u32::Zero)
    ,   m_blockSize(size)
{
    Block* head = 0;
    for (u32 i = 0; i < initialCount; ++i)
    {
        Block* block = (Block*)platformReserve(blockSize());
        platformCommit((byte*)block, 0, blockSize());
        block->next = head;
        head = block;
    }
    m_head.setConditional(head, m_head.getTicket());
}

SystemAllocator::~SystemAllocator()
{
    be_assert(m_used == 0, "Not all blocks reclaimed when system allocator was freed");
    Block* head = m_head;
    while (head)
    {
        byte* buffer = reinterpret_cast<byte*>(head);
        head = head->next;
        platformFree(buffer, blockSize());
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
                u32 extra = m_capacity;
                m_capacity += extra;
                Block* head = 0;
                for (u32 i = 0; i < extra; ++i)
                {
                    Block* block = (Block*)platformReserve(blockSize());
                    platformCommit((byte*)block, 0, blockSize());
                    block->next = head;
                    head = block;
                }
                m_head.setConditional(head, m_head.getTicket());
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

}
