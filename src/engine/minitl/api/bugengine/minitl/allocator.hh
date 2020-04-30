/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_ALLOCATOR_HH_
#define BE_MINITL_ALLOCATOR_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <bugengine/minitl/swap.hh>
#include <cstring>

namespace minitl {

class be_api(MINITL) Allocator
{
public:
    template < typename T >
    class Block
    {
        BE_NOCOPY(Block);

    private:
        Allocator* m_allocator;
        u64        m_count;
        T*         m_data;

    public:
        Block(Allocator& allocator, u64 count, u64 blockAlignment = 4)
            : m_allocator(&allocator)
            , m_count(count)
            , m_data(count ? (T*)allocator.alloc(align(sizeof(T), be_alignof(T)) * count,
                                                 max< u64 >(blockAlignment, be_alignof(T)))
                           : 0) {};
        ~Block()
        {
            m_allocator->free(m_data);
        }
        inline Allocator& arena() const
        {
            return *m_allocator;
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
        u64 count() const
        {
            return m_count;
        }
        u64 byteCount() const
        {
            return align(sizeof(T), be_alignof(T)) * m_count;
        }
        T* begin()
        {
            return m_data;
        }
        T* end()
        {
            return m_data + m_count;
        }
        const T* begin() const
        {
            return m_data;
        }
        const T* end() const
        {
            return m_data + m_count;
        }

        bool resize(u64 count)
        {
            u64 size = align(sizeof(T), be_alignof(T)) * count;
            if(m_allocator->resize(m_data, size))
            {
                m_count = count;
                return true;
            }
            else
            {
                return false;
            }
        }
        void realloc(u64 count, u64 blockAlignment = 4)
        {
            if(count > m_count)
            {
                u64 alignment = max< u64 >(blockAlignment, be_alignof(T));
                u64 size      = align(sizeof(T), be_alignof(T)) * count;
                m_count       = count;
                m_data        = (T*)m_allocator->realloc(m_data, size, alignment);
            }
            else
            {
                // shrink does not realloc
                m_count = count;
                if(!count)
                {
                    m_allocator->free(m_data);
                    m_data = 0;
                }
            }
        }
        void swap(Block< T >& other)
        {
            minitl::swap(m_allocator, other.m_allocator);
            minitl::swap(m_count, other.m_count);
            minitl::swap(m_data, other.m_data);
        }
    };

protected:
    virtual void* internalAlloc(u64 size, u64 alignment)              = 0;
    virtual bool  internalResize(void* ptr, u64 size)                 = 0;
    virtual void* internalRealloc(void* ptr, u64 size, u64 alignment) = 0;
    virtual void  internalFree(const void* pointer)                   = 0;
    virtual ~Allocator()
    {
    }

public:
    inline void* alloc(u64 size, u64 alignment = 4);
    inline bool  resize(void* ptr, u64 size);
    inline void* realloc(void* ptr, u64 size, u64 alignment);
    inline void  free(const void* pointer);
    inline char* strdup(const char* src);
    inline char* strdup(const char* begin, const char* end);
    template < typename T >
    inline T* alloc();
};

void* Allocator::alloc(u64 size, u64 alignment)
{
    return internalAlloc(size, alignment);
}

bool Allocator::resize(void* ptr, u64 size)
{
    return internalResize(ptr, size);
}

void* Allocator::realloc(void* ptr, u64 size, u64 alignment)
{
    return internalRealloc(ptr, size, alignment);
}

void Allocator::free(const void* pointer)
{
    internalFree(pointer);
}

char* Allocator::strdup(const char* src)
{
    size_t s      = strlen(src);
    char*  result = static_cast< char* >(internalAlloc(s + 1, 1));
    strcpy(result, src);
    return result;
}

char* Allocator::strdup(const char* begin, const char* end)
{
    size_t s      = end - begin;
    char*  result = static_cast< char* >(internalAlloc(s + 1, 1));
    strncpy(result, begin, s);
    result[s] = '\0';
    return result;
}

template < typename T >
T* Allocator::alloc()
{
    return (T*)alloc(sizeof(T), be_alignof(T));
}

}  // namespace minitl

#include <new>

inline void* operator new(size_t size, minitl::Allocator& allocator)
{
    return allocator.alloc(size);
}

inline void* operator new(size_t size, minitl::Allocator& allocator, size_t align)
{
    return allocator.alloc(size, align);
}

inline void operator delete(void* ptr, minitl::Allocator& allocator)
{
    allocator.free(ptr);
}

inline void operator delete(void* ptr, minitl::Allocator& allocator, size_t /*align*/)
{
    allocator.free(ptr);
}

inline void* operator new[](size_t size, minitl::Allocator& allocator)
{
    return allocator.alloc(size);
}

inline void* operator new[](size_t size, minitl::Allocator& allocator, size_t align)
{
    return allocator.alloc(size, align);
}

inline void operator delete[](void* ptr, minitl::Allocator& allocator)
{
    allocator.free(ptr);
}

inline void operator delete[](void* ptr, minitl::Allocator& allocator, size_t /*align*/)
{
    allocator.free(ptr);
}

/**************************************************************************************************/
#endif
