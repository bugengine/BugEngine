/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

static SYSTEM_INFO s_systemInfo;
static bool s_systemInfo_acquired = (GetSystemInfo(&s_systemInfo), true);

void* SystemAllocator::platformReserve(u32 size)
{

    be_assert(size % s_systemInfo.dwPageSize == 0, "size %d is not aligned on a page boundary (page size = %d)" | size | s_systemInfo.dwPageSize);
    void* result = VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
    be_assert(result, "failed to reserve memory");
    return result;
}

void* SystemAllocator::platformCommit(void* ptr, u32 start, u32 stop)
{
    be_assert((uintptr_t)ptr % s_systemInfo.dwPageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_systemInfo.dwPageSize);
    be_assert(start % s_systemInfo.dwPageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | start | s_systemInfo.dwPageSize);
    be_assert(stop % s_systemInfo.dwPageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | stop | s_systemInfo.dwPageSize);
    void* result = VirtualAlloc((u8*)ptr + start, stop-start, MEM_COMMIT, PAGE_READWRITE);
    return result;
}

void  SystemAllocator::platformFree(void* ptr, u32 size)
{
    be_assert((uintptr_t)ptr % s_systemInfo.dwPageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_systemInfo.dwPageSize);
    be_assert(size % s_systemInfo.dwPageSize == 0, "size %p is not aligned on a page boundary (page size = %d)" | size | s_systemInfo.dwPageSize);
    VirtualFree(ptr, size, MEM_RELEASE);
}

}
