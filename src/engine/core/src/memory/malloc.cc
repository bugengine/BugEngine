/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/malloc.hh>


namespace BugEngine
{

struct MallocInitializer
{
    bool initialized;
    MallocInitializer()
        :   initialized(false)
    {
#ifdef  BE_ENABLE_MEMORY_TRACKING
# ifdef BE_COMPILER_MSVC
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
# endif
#endif
        initialized = true;
    }
    ~MallocInitializer()
    {
    }
};

static MallocInitializer s_initializer;


template< >
be_api(CORE) void* Memory<Arena::General>::systemAlloc(size_t size, size_t alignment)
{
    be_assert(s_initializer.initialized, "new was called before the memory system was initialized");
#ifdef _MSC_VER
    return _aligned_malloc(size, alignment);
#else
    return malloc(size);
#endif
}

template< >
be_api(CORE) void* Memory<Arena::General>::systemRealloc(void* ptr, size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return _aligned_realloc(ptr, size, alignment);
#else
    return realloc(ptr, size);
#endif
}

template< >
be_api(CORE) void Memory<Arena::General>::systemFree(const void* pointer)
{
#ifdef _MSC_VER
    return _aligned_free(const_cast<void*>(pointer));
#else
    return free(const_cast<void*>(pointer));
#endif
}

}
