/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>
#include    <sys/mman.h>
#include    <unistd.h>
#include    <errno.h>

namespace BugEngine
{

static const u32 s_pageSize = sysconf(_SC_PAGE_SIZE);

void* SystemAllocator::platformReserve(u32 size)
{
    be_assert(size % s_pageSize == 0, "size %d is not aligned on a page boundary (page size = %d)" | size | s_pageSize);
    void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
    be_assert(result, "failed to reserve memory: %s" | sys_errlist[errno]);
    return result;
}

void* SystemAllocator::platformCommit(void* ptr, u32 start, u32 stop)
{
    be_assert((uintptr_t)ptr % s_pageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_pageSize);
    be_assert(start % s_pageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | start | s_pageSize);
    be_assert(stop % s_pageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | stop | s_pageSize);
    mprotect((u8*)ptr + start, stop-start, PROT_READ|PROT_WRITE);
    return (void*)((u8*)ptr + start);
}

void  SystemAllocator::platformFree(void* ptr, u32 size)
{
    be_assert((uintptr_t)ptr % s_pageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_pageSize);
    be_assert(size % s_pageSize == 0, "size %p is not aligned on a page boundary (page size = %d)" | size | s_pageSize);
    munmap(ptr, size);
}

}
