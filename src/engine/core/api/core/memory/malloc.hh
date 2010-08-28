/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_MALLOC_HH_
#define BE_CORE_MEMORY_MALLOC_HH_
/*****************************************************************************/
#include    <cstring>
#include    <core/debug/assert.hh>

namespace BugEngine
{

template< int ARENA >
class Memory
{
private:
    static void* internalAlloc(size_t size, size_t alignment);
    static void* internalRealloc(void* ptr, size_t size, size_t alignment);
    static void  internalFree(const void* pointer);
public:
    static inline void* alloc(size_t size, size_t alignment = 16);
    static inline void* realloc(void* ptr, size_t size, size_t alignment);
    static inline void  free(const void* pointer);
    template< typename T >
    static inline T* allocArray(size_t count, size_t alignment = be_alignof(T));
public:
    template< typename T >
    class Block
    {
    private:
        T* m_data;
    public:
        Block(size_t count, size_t alignment = be_alignof(T))
            :   m_data(Memory<ARENA>::template allocArray<T>(count, alignment))
        {
        };
        ~Block()
        {
            Memory<ARENA>::free(m_data);
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
        void realloc(size_t count, size_t alignment = be_alignof(T))
        {
            size_t size = be_align(sizeof(T),alignment)*count;
            m_data = Memory<ARENA>::realloc(m_data, size, alignment);
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
};

template< int ARENA >
void* Memory<ARENA>::alloc(size_t size, size_t alignment)
{
#ifdef BE_MEMORY_TRACKING
#endif
    return internalAlloc(size, alignment);
}

template< int ARENA >
void* Memory<ARENA>::realloc(void* ptr, size_t size, size_t alignment)
{
    if(!ptr)
        return alloc(size, alignment);
#ifdef BE_MEMORY_TRACKING
#endif
    return internalRealloc(ptr, size, alignment);
}

template< int ARENA >
void  Memory<ARENA>::free(const void* pointer)
{
#ifdef BE_MEMORY_TRACKING
#endif
    internalFree(pointer);
}

template< int ARENA >
template< typename T >
T* Memory<ARENA>::allocArray(size_t count, size_t alignment)
{
    if(!count)
        return 0;
    size_t size = be_align(sizeof(T),alignment)*count;
    void* r = internalAlloc(size, alignment);
    memset(r, 0, size);
    return reinterpret_cast<T*>(r);
}

struct Arena
{
    enum
    {
        // engine section
        General = 0x0000,
        Rtti    = 0x0001,
        // game section
        Game    = 0x0100,
        // plugin section
        Plugin  = 0x1000
    };
};

}

namespace minitl
{
    template< typename T >
    T* advance(T* input, ptrdiff_t offset)
    {
        char *ptr = reinterpret_cast<char*>(input);
        ptr = ptr + be_align(sizeof(T),be_alignof(T))*offset;
        return reinterpret_cast<T*>(ptr);
    }
    template< typename T >
    const T* advance(const T* input, ptrdiff_t offset)
    {
        const char *ptr = reinterpret_cast<const char*>(input);
        ptr = ptr + be_align(sizeof(T),be_alignof(T))*offset;
        return reinterpret_cast<const T*>(ptr);
    }
    template< typename T >
    ptrdiff_t distance(const T* t1, const T* t2)
    {
        const byte *ptr1 = reinterpret_cast<const byte*>(t1);
        const byte *ptr2 = reinterpret_cast<const byte*>(t2);
        ptrdiff_t d = ptr2 - ptr1;
        be_assert(d %  be_align(sizeof(T),be_alignof(T)) == 0, "distance between %p and %p is not a multiple of the size" | t1 | t2);
        return d / be_align(sizeof(T),be_alignof(T));
    }
}


#ifdef BE_PLATFORM_MACOS
# include   <malloc/malloc.h>
#endif
#include    <new>

#ifdef _MSC_VER
# define    malloca    _alloca
# define    freea      _freea
#else
# define    malloca     alloca
# define    freea(p)
#endif


/*****************************************************************************/
#endif

