/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_MALLOC_HH_
#define BE_CORE_MEMORY_MALLOC_HH_
/*****************************************************************************/
#include <cstring>

namespace BugEngine
{

class be_api(CORE) Malloc
{
public:
    static void init();

    static void* systemAlloc(size_t size, size_t alignment);
    static void* systemRealloc(void* ptr, size_t size, size_t alignment);
    static void  systemFree(void* pointer);
    template< typename T >
    static inline T* systemAllocArray(size_t count, size_t alignment = be_alignof(T));
public:
    static void*    internalAlloc(size_t size, size_t alignment, size_t skipStack = 1);
    static void*    internalRealloc(void* ptr, size_t size, size_t alignment, size_t skipStack = 1);
    static void     internalFree(void* ptr, size_t skipStack = 1);
    template< typename T >
    static inline T* internalAllocArray(size_t count, size_t alignment = be_alignof(T));

    static void     frameUpdate();
};

template< typename T >
T* Malloc::internalAllocArray(size_t count, size_t alignment)
{
    size_t size = be_align(sizeof(T),alignment)*count;
    void* r = internalAlloc(size, alignment);
    memset(r, 0, size);
    return reinterpret_cast<T*>(r);
}

template< typename T >
T* Malloc::systemAllocArray(size_t count, size_t alignment)
{
    size_t size = be_align(sizeof(T),alignment)*count;
    void* r = systemAlloc(size, alignment);
    memset(r, 0, size);
    return reinterpret_cast<T*>(r);
}

}

#define be_malloc(s)            BugEngine::Malloc::internalAlloc(s,16)
#define be_malloc_aligned(s,a)  BugEngine::Malloc::internalAlloc(s,a)
#define be_realloc(p,s)         BugEngine::Malloc::internalRealloc(p,s,16)
#define be_free(p)              BugEngine::Malloc::internalFree(p)

#ifdef BE_PLATFORM_MACOS
# include   <malloc/malloc.h>
#else
# include   <malloc.h>
#endif
#include    <new>

#ifdef _MSC_VER
# define    malloca    _alloca
# define    freea      _freea
#else
# define    malloca     alloca
# define    freea(p)
#endif


template< typename T >
static inline void checked_delete(T* ptr)
{
    char typeMustBeComplete[sizeof(T)];
    (void)typeMustBeComplete;
    delete ptr;
}

/*****************************************************************************/
#endif

