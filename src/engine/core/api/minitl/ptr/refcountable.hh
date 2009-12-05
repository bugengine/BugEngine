/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_REFCOUNTABLE_HH_
#define BE_MINITL_PTR_REFCOUNTABLE_HH_
/*****************************************************************************/
#include   <minitl/ptr/pointer.hh>

namespace minitl
{

class refcountable;

template< typename T >
static inline void checked_delete(const T* ptr)
{
    char typeMustBeComplete[sizeof(T)];
    (void)typeMustBeComplete;
    delete ptr;
}

class refcountable : public pointer
{
    friend inline void addref(const refcountable* ptr);
    template < typename T >
    friend inline const T* decref(const T* ptr);
    template< typename T >
    friend class ref;
    template< typename T >
    friend class scoped;
    template < typename T >
    friend inline void checked_delete(const T* ptr);
private:
    mutable i_u32 m_refCount;
public:
    refcountable()
    :   m_refCount(0)
    {}
    inline virtual ~refcountable()
    {
        be_assert(m_refCount == 0, "object is destroyed but has %d references" | m_refCount);
    }
private:
    void  operator&() const;
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
protected:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};

inline void addref(const refcountable* ptr)
{
    if(ptr)
        ++ptr->m_refCount;
}

template< typename T >
inline const T* decref(const T *ptr)
{
    if(!ptr)
        return 0;
    be_assert(ptr->m_refCount > 0, "object has no reference; cannot dereference it again");
    if(! --ptr->m_refCount)
    {
        checked_delete<const T>(ptr);
        return 0;
    }
    else
    {
        return ptr;
    }
}

}

/*****************************************************************************/
#endif
