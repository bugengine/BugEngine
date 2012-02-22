/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_MALLOC_HH_
#define BE_CORE_MEMORY_MALLOC_HH_
/*****************************************************************************/
#include    <core/stdafx.h>
#include    <cstring>
#include    <core/debug/assert.hh>
#include    <minitl/container/algorithm.hh>

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
        /* doesn't handle destructors; make sure there is no complex data in here */
        union PODCheck
        {
            T t;
        };
    private:
        Allocator*  m_allocator;
        size_t      m_count;
        T*          m_data;
    public:
        Block(Allocator& allocator, size_t count, size_t alignment = be_min<size_t>(16, be_alignof(T)))
            :   m_allocator(&allocator)
            ,   m_count(count)
            ,   m_data((T*)allocator.alloc(be_align(sizeof(T), be_alignof(T))*count, alignment))
        {
        };
        ~Block()
        {
            m_allocator->free(m_data);
        }
        inline Allocator& arena() const { return *m_allocator; }
        T* data()                       { return m_data; }
        const T* data() const           { return m_data; }
        operator T*()                   { return m_data; }
        operator const T*() const       { return m_data; }
        size_t count() const            { return m_count; }
        size_t byteCount() const        { return be_align(sizeof(T), be_alignof(T))*m_count; }

        bool resize(size_t count)
        {
            size_t size = be_align(sizeof(T), be_alignof(T))*count;
            if (m_allocator->resize(m_data, size))
            {
                m_count = count;
                return true;
            }
            else
            {
                return false;
            }
        }
        void realloc(size_t count, size_t alignment = be_min<size_t>(16, be_alignof(T)))
        {
            size_t size = be_align(sizeof(T), be_alignof(T))*count;
            m_count = count;
            m_data = (T*)m_allocator->realloc(m_data, size, alignment);
        }
        void swap(Block<T>& other)
        {
            minitl::swap(m_allocator, other.m_allocator);
            minitl::swap(m_count, other.m_count);
            minitl::swap(m_data, other.m_data);
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
    inline const char* strdup(const char *src);
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

const char*  Allocator::strdup(const char* src)
{
#ifdef BE_MEMORY_TRACKING
#endif
    size_t s = strlen(src);
    char *result = static_cast<char*>(internalAlloc(s+1, 1));
    strcpy(result, src);
    return result;
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
be_api(CORE) Allocator& inputArena();

}


#include    <new>

inline void* operator new(size_t size, BugEngine::Allocator& allocator)                     { return allocator.alloc(size); }
inline void* operator new(size_t size, BugEngine::Allocator& allocator, size_t align)       { return allocator.alloc(size, align); }
inline void  operator delete(void* ptr, BugEngine::Allocator& allocator)                    { allocator.free(ptr); }
inline void  operator delete(void* ptr, BugEngine::Allocator& allocator, size_t /*align*/)  { allocator.free(ptr); }

/*****************************************************************************/
#endif

