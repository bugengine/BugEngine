/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/malloc.hh>

#include    <malloc.h>


namespace BugEngine
{

bool s_initialized = false;

void Malloc::init()
{
    if(!s_initialized)
    {
        s_initialized = true;
        //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
        Logger::root();
    }
#ifdef BE_ENABLE_MEMORY_TRACKING
#endif
}


void* Malloc::systemAlloc(size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return _aligned_malloc(size, alignment);
#else
    return malloc(size);
#endif
}

void* Malloc::systemRealloc(void* ptr, size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return _aligned_realloc(ptr, size, alignment);
#else
    return realloc(ptr, size);
#endif
}

void Malloc::systemFree(void* pointer)
{
#ifdef _MSC_VER
    return _aligned_free(pointer);
#else
    return free(pointer);
#endif
}

BE_NOINLINE void* Malloc::internalAlloc(size_t size, size_t alignment, size_t /*skipStack*/)
{
    be_assert(s_initialized, "new was called before the memory system was initialized");
    void* ptr = systemAlloc(size, alignment);
#ifdef BE_ENABLE_MEMORY_TRACKING
#endif
    return ptr;
}

BE_NOINLINE void* Malloc::internalRealloc(void* ptr, size_t size, size_t alignment, size_t /*skipStack*/)
{
#ifdef BE_ENABLE_MEMORY_TRACKING
#endif
    ptr = systemRealloc(ptr, size, alignment);
#ifdef BE_ENABLE_MEMORY_TRACKING
#endif
    return ptr;
}

void Malloc::internalFree(void* ptr, size_t /*skipStack*/)
{
    if(! ptr)
        return;
#ifdef BE_ENABLE_MEMORY_TRACKING
#endif
    systemFree(ptr);
}

void Malloc::frameUpdate()
{
#ifdef BE_ENABLE_MEMORY_TRACKING
#endif
}

}
