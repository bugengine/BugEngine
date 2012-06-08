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
private:
    byte* const m_buffer;
    u32 const   m_capacity;
    u32         m_usage;
    u32         m_realUsage;
private:
    static u32   platformPageSize();
    static byte* platformReserve(u32 size);
    static void  platformCommit(byte* ptr, u32 begin, u32 end);
    static void  platformRelease(byte* ptr, u32 begin, u32 end);
    static void  platformFree(byte* ptr, u32 size);
public:
    /**
     * Creates a new contiguous memory region of maximumBytes size.
     * The allocator will actually reserve a contiguous space for maximumBytes but won't commit all
     * the memory. The committed memory will grow when needed.
     * @param maximumBytes maximum number of bytes than can be used.
     */
    SystemAllocator(u32 maximumBytes);
    /**
     * Destroys the pool. Asserts in case some of the memory in the pool has not been released.
     */
    ~SystemAllocator();
    /**
     * Returns a pointer to the beginning of the buffer.
     * \returns A pointer to the buffer.
     */
    byte* buffer()
    {
        return m_buffer;
    }
    /**
     * Set the current needed memory. If the needed memory decreases, some pages will become
     * available again. If the needed memory increases, some pages will be requested to the
     * operating system.
     * \param byteCount The current needed memory usage.
     */
    void  setUsage(u32 byteCount);
};

}

/*****************************************************************************/
#endif

