/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/memory/allocators/pool.hh>

namespace BugEngine {

// TODO
PoolAllocator::PoolAllocator()
{
}

PoolAllocator::~PoolAllocator()
{
}

void* PoolAllocator::internalAlloc(u64 size, u64 alignment)
{
#ifdef _MSC_VER
    return ::_aligned_malloc(be_checked_numcast< size_t >(size),
                             be_checked_numcast< size_t >(alignment));
#else
    be_forceuse(alignment);
    return ::malloc(size);
#endif
}

bool PoolAllocator::internalResize(void* /*ptr*/, u64 /*size*/)
{
    return false;
}

void* PoolAllocator::internalRealloc(void* ptr, u64 size, u64 alignment)
{
#ifdef _MSC_VER
    return ::_aligned_realloc(ptr, be_checked_numcast< size_t >(size),
                              be_checked_numcast< size_t >(alignment));
#else
    be_forceuse(alignment);
    return ::realloc(ptr, size);
#endif
}

void PoolAllocator::internalFree(const void* pointer)
{
#ifdef _MSC_VER
    ::_aligned_free(const_cast< void* >(pointer));
#else
    ::free(const_cast< void* >(pointer));
#endif
}

}  // namespace BugEngine
