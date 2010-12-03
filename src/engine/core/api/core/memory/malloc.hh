/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_MALLOC_HH_
#define BE_CORE_MEMORY_MALLOC_HH_
/*****************************************************************************/
#include    <cstring>
#include    <core/debug/assert.hh>

namespace BugEngine
{

class be_api(CORE) Allocator
{
public:
    template< typename T >
    class Block
    {
    private:
        Allocator&  m_allocator;
        T*          m_data;
    public:
        Block(Allocator& allocator, size_t count, size_t alignment = be_alignof(T))
            :   m_allocator(allocator)
            ,   m_data(allocator.allocArray<T>(count, alignment))
        {
        };
        ~Block()
        {
            m_allocator.free(m_data);
        }
        inline Allocator& arena()
        {
            return m_allocator;
        }
        T* data()
        {
            return m_data;
        }
        const T* data() const
        {
            return m_data;
        }
        operator T*()
        {
            return m_data;
        }
        operator const T*() const
        {
            return m_data;
        }
        bool resize(size_t count, size_t alignment = be_alignof(T))
        {
            size_t size = be_align(sizeof(T),alignment)*count;
            return m_allocator.resize(m_data, size);
        }
        void realloc(size_t count, size_t alignment = be_alignof(T))
        {
            size_t size = be_align(sizeof(T),alignment)*count;
            m_data = (T*)m_allocator.realloc(m_data, size);
        }
        void swap(Block<T>& other)
        {
            T* data = m_data;
            m_data = other.m_data;
            other.m_data = data;
        }
    private:
        Block(const Block& other);
        Block& operator=(const Block& other);
    };
private:
    virtual void* internalAlloc(size_t size, size_t alignment) = 0;
    virtual bool  internalResize(void* ptr, size_t size) = 0;
    virtual void* internalRealloc(void* ptr, size_t size) = 0;
    virtual void  internalFree(const void* pointer) = 0;
public:
    inline void* alloc(size_t size, size_t alignment = 16);
    inline bool  resize(void* ptr, size_t size);
    inline void* realloc(void* ptr, size_t size);
    inline void  free(const void* pointer);
    template< typename T >
    inline T* allocArray(size_t count, size_t alignment = be_alignof(T));
};

void* Allocator::alloc(size_t size, size_t alignment)
{
#ifdef BE_MEMORY_TRACKING
#endif
    return internalAlloc(size, alignment);
}

bool  Allocator::resize(void* ptr, size_t size)
{
#ifdef BE_MEMORY_TRACKING
#endif
    return internalResize(ptr, size);
}

void* Allocator::realloc(void* ptr, size_t size)
{
#ifdef BE_MEMORY_TRACKING
#endif
    return internalRealloc(ptr, size);
}

void  Allocator::free(const void* pointer)
{
#ifdef BE_MEMORY_TRACKING
#endif
    internalFree(pointer);
}

template< typename T >
T* Allocator::allocArray(size_t count, size_t alignment)
{
    if(!count)
        return 0;
    size_t size = be_align(sizeof(T),alignment)*count;
    void* r = internalAlloc(size, alignment);
    memset(r, 0, size);
    return reinterpret_cast<T*>(r);
}

be_api(CORE) Allocator& gameArena();
be_api(CORE) Allocator& tempArena();
be_api(CORE) Allocator& debugArena();
be_api(CORE) Allocator& rttiArena();

}


#ifdef BE_PLATFORM_MACOS
# include   <malloc/malloc.h>
#else
# include   <stdlib.h>
#endif
#ifdef BE_PLATFORM_SUN
# include   <alloca.h>
#endif
#include    <new>

#ifdef _MSC_VER
# define    malloca    _alloca
# define    freea      _freea
#else
# if !defined(BE_PLATFORM_MACOS)
#  include <malloc.h>
# endif
# define    malloca     alloca
# define    freea(p)
#endif


/*****************************************************************************/
#endif

