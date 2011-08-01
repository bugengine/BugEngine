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
        BE_NOCOPY(Block);
    private:
        Allocator&  m_allocator;
        T*          m_data;
    public:
        Block(Allocator& allocator, size_t count, size_t alignment = be_alignof(T))
            :   m_allocator(allocator)
            ,   m_data((T*)allocator.alloc(be_align(sizeof(T), alignment)*count, alignment))
        {
        };
        ~Block()
        {
            m_allocator.free(m_data);
        }
        inline Allocator& arena() const
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
            size_t size = be_align(sizeof(T), alignment)*count;
            return m_allocator.resize(m_data, size);
        }
        void realloc(size_t count, size_t alignment = be_alignof(T))
        {
            size_t size = be_align(sizeof(T), alignment)*count;
            m_data = (T*)m_allocator.realloc(m_data, size, alignment);
        }
        void swap(Block<T>& other)
        {
            T* data = m_data;
            m_data = other.m_data;
            other.m_data = data;
        }
    };
protected:
    virtual void* internalAlloc(size_t size, size_t alignment) = 0;
    virtual bool  internalResize(void* ptr, size_t size) = 0;
    virtual void* internalRealloc(void* ptr, size_t size, size_t alignment) = 0;
    virtual void  internalFree(const void* pointer) = 0;
    virtual ~Allocator() { }
public:
    inline void* alloc(size_t size, size_t alignment = 16);
    inline bool  resize(void* ptr, size_t size);
    inline void* realloc(void* ptr, size_t size, size_t alignment);
    inline void  free(const void* pointer);
    template< typename T >
    inline T* alloc();
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

void* Allocator::realloc(void* ptr, size_t size, size_t alignment)
{
#ifdef BE_MEMORY_TRACKING
#endif
    return internalRealloc(ptr, size, alignment);
}

void  Allocator::free(const void* pointer)
{
#ifdef BE_MEMORY_TRACKING
#endif
    internalFree(pointer);
}

template< typename T >
T* Allocator::alloc()
{
    return (T*)alloc(sizeof(T), be_alignof(T));
}

be_api(CORE) Allocator& gameArena();
be_api(CORE) Allocator& tempArena();
be_api(CORE) Allocator& debugArena();
be_api(CORE) Allocator& rttiArena();
be_api(CORE) Allocator& scriptArena();
be_api(CORE) Allocator& taskArena();
be_api(CORE) Allocator& inputArena();

}


#ifdef BE_PLATFORM_MACOS
# include   <malloc/malloc.h>
#else
# include   <stdlib.h>
#endif
#ifdef BE_PLATFORM_SUN
# include   <alloca.h>
#endif

#ifdef _MSC_VER
# define    malloca    _alloca
# if _MSC_VER >= 1400
#  define    freea      _freea
# else
#  define    freea(p)
# endif
#else
# if !defined(BE_PLATFORM_MACOS) && !defined(BE_PLATFORM_BSD)
#  include <malloc.h>
# endif
# define    malloca     alloca
# define    freea(p)
#endif

#include    <new>

inline void* operator new(size_t size, BugEngine::Allocator& allocator)                     { return allocator.alloc(size); }
inline void* operator new(size_t size, BugEngine::Allocator& allocator, size_t align)       { return allocator.alloc(size, align); }
inline void  operator delete(void* ptr, BugEngine::Allocator& allocator)                    { return allocator.free(ptr); }
inline void  operator delete(void* ptr, BugEngine::Allocator& allocator, size_t /*align*/)  { return allocator.free(ptr); }

/*****************************************************************************/
#endif

