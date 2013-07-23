/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>
#include    <sys/mman.h>
#include    <unistd.h>
#include    <errno.h>
#include    <stdio.h>

namespace BugEngine
{

static const u32 s_pageSize = sysconf(_SC_PAGE_SIZE);
static const u32 s_cacheAhead =
#if BE_ENABLE_MEMORY_DEBUGGING
        0;
#else
        s_pageSize;
#endif

u32 SystemAllocator::platformPageSize()
{
    return s_pageSize;
}

byte* SystemAllocator::platformReserve(u32 size)
{
    be_assert(size % s_pageSize == 0, "size %d is not aligned on a page boundary (page size = %d)" | size | s_pageSize);
    byte* result = (byte*)mmap(0, size+s_pageSize, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
    be_assert(result, "failed to reserve memory for %d bytes: %s" | size | strerror(errno));
#if !BE_ENABLE_MEMORY_DEBUGGING
    int failed = mprotect(result, s_cacheAhead, PROT_READ|PROT_WRITE);
    be_forceuse(failed);
    be_assert(failed == 0, "failed to reserve memory for %d bytes at offset %d: %s" | size | 0 | sys_errlist[errno]);
#endif
    return result;
}

void SystemAllocator::platformCommit(byte* ptr, u32 begin, u32 end)
{
    be_assert((intptr_t)ptr % s_pageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_pageSize);
    be_assert(begin % s_pageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | begin | s_pageSize);
    be_assert(end % s_pageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | end | s_pageSize);
    int failed = mprotect((char*)ptr + begin + s_cacheAhead, end-begin, PROT_READ|PROT_WRITE);
    be_forceuse(failed);
    be_assert(failed == 0, "failed to commit memory for %d bytes at offset %d: %s" | (end-begin) | begin | strerror(errno));
}

void  SystemAllocator::platformRelease(byte* ptr, u32 begin, u32 end)
{
    be_assert((intptr_t)ptr % s_pageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_pageSize);
    be_assert(begin % s_pageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | begin | s_pageSize);
    be_assert(end % s_pageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | end | s_pageSize);
    int failed = mprotect((char*)ptr + begin + s_cacheAhead, end-begin, PROT_NONE);
    be_forceuse(failed);
    be_assert(failed == 0, "failed to release memory for %d bytes at offset %d: %s" | (end-begin) | begin | strerror(errno));
}

void  SystemAllocator::platformFree(byte* ptr, u32 size)
{
    be_assert((intptr_t)ptr % s_pageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_pageSize);
    be_assert(size % s_pageSize == 0, "size %p is not aligned on a page boundary (page size = %d)" | size | s_pageSize);
    int failed;
#if !BE_ENABLE_MEMORY_DEBUGGING
    failed = mprotect(ptr, s_cacheAhead, PROT_NONE);
    be_forceuse(failed);
    be_assert(failed == 0, "failed to release memory for %d bytes at offset %d: %s" | (end-begin) | begin | strerror(errno));
#endif
    failed = munmap((char*)ptr, size);
    be_forceuse(failed);
    be_assert(failed == 0, "failed to unmap memory for %d bytes: %s" | size | strerror(errno));
}

}
