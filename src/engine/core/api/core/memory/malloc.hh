/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_MALLOC_HH_
#define BE_CORE_MEMORY_MALLOC_HH_
/*****************************************************************************/
#include <cstring>

namespace BugEngine
{

template< int ARENA >
class Memory
{
private:
    static void* systemAlloc(size_t size, size_t alignment);
    static void* systemRealloc(void* ptr, size_t size, size_t alignment);
    static void  systemFree(const void* pointer);
public:
    static inline void* alloc(size_t size, size_t alignment);
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
        T& operator[](const size_t index)
        {
            return m_data[index];
        }
        const T& operator[](const size_t index) const
        {
            return m_data[index];
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
    return systemAlloc(size, alignment);
}

template< int ARENA >
void* Memory<ARENA>::realloc(void* ptr, size_t size, size_t alignment)
{
    if(!ptr)
        return alloc(size, alignment);
#ifdef BE_MEMORY_TRACKING
#endif
    return systemRealloc(ptr, size, alignment);
}

template< int ARENA >
void  Memory<ARENA>::free(const void* pointer)
{
#ifdef BE_MEMORY_TRACKING
#endif
    systemFree(pointer);
}

template< int ARENA >
template< typename T >
T* Memory<ARENA>::allocArray(size_t count, size_t alignment)
{
    size_t size = be_align(sizeof(T),alignment)*count;
    void* r = systemAlloc(size, alignment);
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

