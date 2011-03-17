/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/pool.hh>

namespace BugEngine
{

//TODO
PoolAllocator::PoolAllocator()
{
}

PoolAllocator::~PoolAllocator()
{
}

void* PoolAllocator::internalAlloc(size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_malloc(size, alignment);
#else
    return ::malloc(size);
#endif
}

bool PoolAllocator::internalResize(void* /*ptr*/, size_t /*size*/)
{
    return false;
}

void* PoolAllocator::internalRealloc(void* ptr, size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_realloc(ptr, size, alignment);
#else
    return ::realloc(ptr, size);
#endif
}

void PoolAllocator::internalFree(const void* pointer)
{
#ifdef _MSC_VER
    return ::_aligned_free(const_cast<void*>(pointer));
#else
    return ::free(const_cast<void*>(pointer));
#endif
}

}
