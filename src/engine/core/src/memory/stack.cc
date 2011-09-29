/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/stack.hh>

namespace BugEngine
{

StackAllocator::StackAllocator()
{
}

StackAllocator::~StackAllocator()
{
}

void* StackAllocator::internalAlloc(size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_malloc(size, alignment);
#else
    return ::malloc(size);
#endif
}

bool StackAllocator::internalResize(void* /*ptr*/, size_t /*size*/)
{
    return false;
}

void* StackAllocator::internalRealloc(void* ptr, size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_realloc(ptr, size, alignment);
#else
    be_forceuse(alignment);
    return ::realloc(ptr, size);
#endif
}

void StackAllocator::internalFree(const void* pointer)
{
#ifdef _MSC_VER
    ::_aligned_free(const_cast<void*>(pointer));
#else
    ::free(const_cast<void*>(pointer));
#endif
}

}
