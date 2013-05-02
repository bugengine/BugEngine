/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

SystemAllocator::SystemAllocator(BlockSize size, u32 initialCount)
    :   m_head(0)
    ,   m_capacity(initialCount)
    ,   m_used(0)
    ,   m_blockSize(size)
{
    for (u32 i = 0; i < initialCount; ++i)
    {
        Block* block = (Block*)platformReserve(blockSize());
        platformCommit((byte*)block, 0, blockSize());
        block->next = m_head;
        m_head = block;
    }
}

SystemAllocator::~SystemAllocator()
{
    be_assert(m_used == 0, "Not all blocks reclaimed when system allocator was freed");
    while (m_head)
    {
        byte* buffer = (byte*)m_head;
        m_head = m_head->next;
        platformFree(buffer, blockSize());
    }
}

void* SystemAllocator::allocate()
{
    if (m_used < m_capacity)
    {
        ++ m_used;
        void* result = m_head;
        if (! result)
        {
            /* TODO */
        }
        m_head = m_head->next;
        return result;
    }
    else
    {
        return 0;
    }
}

void  SystemAllocator::free(void* memory)
{
    -- m_used;
    Block* newblock = (Block*)memory;
    newblock->next = m_head;
    m_head = newblock;
}

u32 SystemAllocator::blockSize() const
{
    return 1 << (((u32)m_blockSize * 2) + 12);
}

}
