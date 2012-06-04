/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_ALLOCATORS_SYSTEM_HH_
#define BE_CORE_MEMORY_ALLOCATORS_SYSTEM_HH_
/*****************************************************************************/
#include    <core/memory/malloc.hh>

namespace BugEngine
{

/// Allocates regions of contiguous virtual memory and divides it in blocks of equal size
/**
 * The system allocator allocates big blocks of memory in virtual memory and divides it
 * in blocks of several kilobytes.
 * It is suitable mostly for big arrays of data.
 * Use with caution; on 32 bits systms, it can quickly run out of virtual address space.
 */
class be_api(CORE) SystemAllocator
{
public:
    /**
     * Convenience enum to allocate memory.
     * Since it is only possible to allocate a memory which size is a power of two,
     * this enum makes sure that no invalid value can be passed to \ref systeAlloc.
     */
    enum BlockSize
    {
        Block16kb = 14,         //< reserve blocks of 16 kilobytes (the minimum granularity)
        Block32kb = 15,         //< reserve blocks of 32 kilobytes
        Block64kb = 16,         //< reserve blocks of 64 kilobytes
        Block128kb = 17,        //< reserve blocks of 128 kilobytes
        Block256kb = 18,        //< reserve blocks of 256 kilobytes (the maximum granularity)
        BlockMax = Block256kb   //< Alias to the maximum bucket size
    };
private:
    minitl::itaggedptr<void>    m_pool;
    void* const                 m_buffer;
    BlockSize const             m_blockSize;
    i_u32                       m_blockCount;
    u32 const                   m_maximumBlockCount;
    i_u32                       m_usedBlockCount;
private:
    static void* platformReserve(u32 size);
    static void* platformCommit(void* ptr, u32 start, u32 stop);
    static void  platformFree(void* ptr, u32 size);
    void grow();
public:
    /**
     * Creates a new pool of maximumBlockCount blocks. The blocks will all have a size of blockSize.
     * The allocator will actually reserve a contiguous space for maximumBlockCount but won't commit all
     * the memory. The committed memory will grow when running out of blocks.
     * @param blockSize Size of each individual block.
     * @param maximumBlockCount Maximum amount of blocks that can be allocated.
     */
    SystemAllocator(BlockSize blockSize, u32 maximumBlockCount);
    /**
     * Destroys the pool. Asserts in case some of the memory in the pool has not been released.
     */
    ~SystemAllocator();
    /**
     * Allocates size kilobytes of contiguous memory.
     * If no block is available, returns 0.
     * \returns A pointer to a block of free memory.
     */
    byte* blockAlloc();
    /**
     * Frees the system memory and place it back in the list of free memory blocks.
     * \param ptr A pointer to the beginning of the block.
     */
    void  blockFree(byte* ptr);

    /**
     * Returns the block size (in bytes) that this allocator has been configured for
     */
    u32 blockSize() const
    {
        return 1 << ((u32)m_blockSize);
    }
};

}

/*****************************************************************************/
#endif

