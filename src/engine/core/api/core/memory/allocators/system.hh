/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_ALLOCATORS_SYSTEM_HH_
#define BE_CORE_MEMORY_ALLOCATORS_SYSTEM_HH_
/**************************************************************************************************/
#include    <core/stdafx.h>
#include    <minitl/allocator.hh>
#include    <core/threads/mutex.hh>

namespace BugEngine
{

/// Allocates regions of memory.
/**
 * The system allocator allocates blocks of a large size to store objects contiguously.
 * On platforms that support it, it uses page allocations.
 * When all blocks are consumed, the allocator will reserve more memory. When blocks are freed,
 * the allocator does not return them to the system.
 */
class be_api(CORE) SystemAllocator
{
public:
    enum BlockSize
    {
        BlockSize_4k    = 0,
        BlockSize_16k   = 1,
        BlockSize_64k   = 2,
        BlockSize_256k  = 3
    };
private:
    struct Block
    {
        itaggedptr<Block> next;
    };
    itaggedptr<Block>   m_head;
    i_u32               m_capacity;
    i_u32               m_used;
    const BlockSize     m_blockSize;
    Mutex               m_allocLock;
private:
    u32     platformPageSize();
    byte*   platformReserve(u32 size);
    void    platformCommit(byte* ptr, u32 start, u32 stop);
    void    platformRelease(byte* ptr, u32 start, u32 stop);
    void    platformFree(byte* ptr, u32 size);
public:
    SystemAllocator(BlockSize size, u32 initialCount);
    ~SystemAllocator();

    void* allocate();
    void  free(void* memory);

    u32 blockSize() const;
private:
    SystemAllocator(const SystemAllocator& other);
    SystemAllocator& operator=(const SystemAllocator& other);
};

}

/**************************************************************************************************/
#endif
