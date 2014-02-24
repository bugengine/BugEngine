/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_ALLOCATORS_SYSTEM_HH_
#define BE_CORE_MEMORY_ALLOCATORS_SYSTEM_HH_
/**************************************************************************************************/
#include    <minitl/allocator.hh>

namespace BugEngine
{

/// Allocates regions of memory.
/**
 * The system allocator allocates blocks of a large size to store contiguous objects.
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
        Block* next;
    };
    Block*      m_head;
    u32         m_capacity;
    u32         m_used;
    BlockSize   m_blockSize;
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
};

}

/**************************************************************************************************/
#endif

