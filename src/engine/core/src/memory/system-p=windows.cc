/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

static SYSTEM_INFO s_systemInfo;
static bool s_systemInfo_acquired = (GetSystemInfo(&s_systemInfo), true);

u32 SystemAllocator::platformPageSize()
{
    return s_systemInfo.dwPageSize;
}

byte* SystemAllocator::platformReserve(u32 size)
{
    be_assert(size % s_systemInfo.dwPageSize == 0, "size %d is not aligned on a page boundary (page size = %d)" | size | s_systemInfo.dwPageSize);
    byte* result = (byte*)VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
    be_assert(result, "failed to reserve memory");
    return result;
}

void SystemAllocator::platformCommit(byte* ptr, u32 start, u32 stop)
{
    be_assert((uintptr_t)ptr % s_systemInfo.dwPageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_systemInfo.dwPageSize);
    be_assert(start % s_systemInfo.dwPageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | start | s_systemInfo.dwPageSize);
    be_assert(stop % s_systemInfo.dwPageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | stop | s_systemInfo.dwPageSize);
    VirtualAlloc(ptr+start, stop-start, MEM_COMMIT, PAGE_READWRITE);
}

void  SystemAllocator::platformRelease(byte* ptr, u32 start, u32 stop)
{
    be_assert((uintptr_t)ptr % s_systemInfo.dwPageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_systemInfo.dwPageSize);
    be_assert(start % s_systemInfo.dwPageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | start | s_systemInfo.dwPageSize);
    be_assert(stop % s_systemInfo.dwPageSize == 0, "offset %d is not aligned on a page boundary (page size = %d)" | stop | s_systemInfo.dwPageSize);
    VirtualFree(ptr+start, stop-start, MEM_DECOMMIT);
}

void  SystemAllocator::platformFree(byte* ptr, u32 size)
{
    be_assert((uintptr_t)ptr % s_systemInfo.dwPageSize == 0, "pointer %p is not aligned on a page boundary (page size = %d)" | ptr | s_systemInfo.dwPageSize);
    be_assert(size % s_systemInfo.dwPageSize == 0, "size %p is not aligned on a page boundary (page size = %d)" | size | s_systemInfo.dwPageSize);
    VirtualFree(ptr, size, MEM_RELEASE);
}

}
