/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_POINTER_HH_
#define BE_MINITL_POINTER_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <bugengine/minitl/allocator.hh>

namespace minitl {

template < typename U, typename T >
static inline U* checkIsA(T* t)
{
    return t;
}

template < typename T >
static inline void checked_destroy(const T* ptr)
{
    char typeMustBeComplete[sizeof(T)];
    (void)typeMustBeComplete;
    if(ptr)
    {
        ptr->~T();
    }
}

template < typename T >
class ref;
template < typename T >
class weak;
template < typename T >
class scoped;
class Arena;

class pointer
{
    template < typename T >
    friend class ref;
    template < typename T >
    friend class weak;
    template < typename T >
    friend class scoped;

private:
    mutable Allocator* m_allocator;
#if BE_ENABLE_WEAKCHECK
    mutable i_u32 m_weakCount;
#endif
public:
    pointer()
        : m_allocator(0)
#if BE_ENABLE_WEAKCHECK
        , m_weakCount(i_u32::create(0))
#endif
    {
    }
    inline virtual ~pointer()
    {
#if BE_ENABLE_WEAKCHECK
        be_assert(m_weakCount == 0, "object is destroyed but has %d weak references" | m_weakCount);
#endif
    }

public:
    void operator delete(void* memory)
    {
        return ::operator delete(memory);
    }
    void operator delete(void* memory, void* where)
    {
        ::operator delete(memory, where);
    }

private:
    void  operator&() const;
    void* operator new(size_t size)
    {
        return ::operator new(size);
    }
    void* operator new(size_t size, void* where)
    {
        return ::operator new(size, where);
    }

private:  // entity behavior
    pointer(const pointer& other);
    pointer& operator=(const pointer& other);

protected:
    inline void checked_delete() const
    {
        Allocator* d = m_allocator;
        checked_destroy(this);
        d->free(this);
    }
#if BE_ENABLE_WEAKCHECK
    inline void addweak() const
    {
        ++m_weakCount;
    }
    inline void decweak() const
    {
        be_assert(m_weakCount, "object has no weak reference; cannot dereference it again");
        --m_weakCount;
    }
#endif
};

template < typename T >
struct RefWrapper
{
    T& m_t;
    RefWrapper(T& t) : m_t(t)
    {
    }
    operator T&() const
    {
        return m_t;
    }
};
template < typename T >
inline RefWrapper< T > byref(T& t)
{
    return RefWrapper< T >(t);
}

}  // namespace minitl

/**************************************************************************************************/
#endif
