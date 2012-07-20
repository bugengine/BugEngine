/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_ALLOCATOR_HH_
#define BE_MINITL_ALLOCATOR_HH_
/*****************************************************************************/
#include    <minitl/stdafx.h>
#include    <cstring>
#include    <debug/assert.hh>
#include    <minitl/algorithm.hh>

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
        u64         m_count;
        T*          m_data;
    public:
        Block(Allocator& allocator, u64 count, u64 alignment = be_alignof(T))
            :   m_allocator(&allocator)
            ,   m_count(count)
            ,   m_data((T*)allocator.alloc(be_align(sizeof(T), be_alignof(T))*count, be_min<u64>(16, alignment)))
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
        u64 count() const               { return m_count; }
        u64 byteCount() const           { return be_align(sizeof(T), be_alignof(T))*m_count; }

        bool resize(u64 count)
        {
            u64 size = be_align(sizeof(T), be_alignof(T))*count;
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
        void realloc(u64 count, u64 alignment = be_min<u64>(16, be_alignof(T)))
        {
            u64 size = be_align(sizeof(T), be_alignof(T))*count;
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
    virtual void* internalAlloc(u64 size, u64 alignment) = 0;
    virtual bool  internalResize(void* ptr, u64 size) = 0;
    virtual void* internalRealloc(void* ptr, u64 size, u64 alignment) = 0;
    virtual void  internalFree(const void* pointer) = 0;
    virtual ~Allocator() { }
public:
    inline void* alloc(u64 size, u64 alignment = 16);
    inline bool  resize(void* ptr, u64 size);
    inline void* realloc(void* ptr, u64 size, u64 alignment);
    inline void  free(const void* pointer);
    inline const char* strdup(const char *src);
    template< typename T >
    inline T* alloc();
};

void* Allocator::alloc(u64 size, u64 alignment)
{
    return internalAlloc(size, alignment);
}

bool  Allocator::resize(void* ptr, u64 size)
{
    return internalResize(ptr, size);
}

void* Allocator::realloc(void* ptr, u64 size, u64 alignment)
{
    return internalRealloc(ptr, size, alignment);
}

void  Allocator::free(const void* pointer)
{
    internalFree(pointer);
}

const char*  Allocator::strdup(const char* src)
{
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

}


#include    <new>

inline void* operator new(size_t size, BugEngine::Allocator& allocator)                         { return allocator.alloc(size); }
inline void* operator new(size_t size, BugEngine::Allocator& allocator, size_t align)           { return allocator.alloc(size, align); }
inline void  operator delete(void* ptr, BugEngine::Allocator& allocator)                        { allocator.free(ptr); }
inline void  operator delete(void* ptr, BugEngine::Allocator& allocator, size_t /*align*/)      { allocator.free(ptr); }
inline void* operator new[](size_t size, BugEngine::Allocator& allocator)                       { return allocator.alloc(size); }
inline void* operator new[](size_t size, BugEngine::Allocator& allocator, size_t align)         { return allocator.alloc(size, align); }
inline void  operator delete[](void* ptr, BugEngine::Allocator& allocator)                      { allocator.free(ptr); }
inline void  operator delete[](void* ptr, BugEngine::Allocator& allocator, size_t /*align*/)    { allocator.free(ptr); }

/*****************************************************************************/
#endif

