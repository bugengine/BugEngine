/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/system.hh>

namespace BugEngine
{

u32 SystemAllocator::platformPageSize()
{
    return 1;
}

byte* SystemAllocator::platformReserve(u32 size)
{
    return (byte*)malloc(size);
}

void  SystemAllocator::platformCommit(byte* ptr, u32 begin, u32 end)
{
    be_forceuse(ptr);
    be_forceuse(begin);
    be_forceuse(end);
}

void  SystemAllocator::platformRelease(byte* ptr, u32 begin, u32 end)
{
    be_forceuse(begin);
    be_forceuse(end);
    free(ptr);
}

void  SystemAllocator::platformFree(byte* ptr, u32 size)
{
    be_forceuse(size);
    free(ptr);
}

}
