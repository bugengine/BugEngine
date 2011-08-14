/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/general.hh>

#ifdef BE_COMPILER_MSVC
#include    <crtdbg.h>
#endif

namespace BugEngine
{

GeneralAllocator::GeneralAllocator()
{
#ifdef  BE_ENABLE_MEMORY_TRACKING
# ifdef BE_COMPILER_MSVC
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
        //_crtBreakAlloc = 1405;
# endif
#endif
}

GeneralAllocator::~GeneralAllocator()
{
}

void* GeneralAllocator::internalAlloc(size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_malloc(size, alignment);
#else
    return ::malloc(size);
#endif
}

bool GeneralAllocator::internalResize(void* /*ptr*/, size_t /*size*/)
{
    return false;
}

void* GeneralAllocator::internalRealloc(void* ptr, size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_realloc(ptr, size, alignment);
#else
    return ::realloc(ptr, size);
#endif
}

void GeneralAllocator::internalFree(const void* pointer)
{
#ifdef _MSC_VER
    return ::_aligned_free(const_cast<void*>(pointer));
#else
    return ::free(const_cast<void*>(pointer));
#endif
}

}
