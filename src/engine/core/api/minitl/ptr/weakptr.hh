/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_WEAKPTR_
#define BE_MINITL_PTR_WEAKPTR_
/*****************************************************************************/
#include <minitl/ptr/refcountable.hh>
#include <minitl/ptr/scopedptr.hh>
#include <minitl/ptr/refptr.hh>


namespace minitl
{

template< typename T >
class weak
{
    template< typename U, typename V >
    friend weak<U> be_checked_cast(weak<V> v);
    template< typename U, typename V >
    friend weak<U> be_const_cast(weak<V> v);
private:
    T* m_ptr;
private:
    void swap(weak& other)
    {
        T* tmp = other.m_ptr;
        other.m_ptr = m_ptr;
        m_ptr = tmp; 
    }
public:
    weak() : m_ptr(0) {}
    weak(T* ptr) : m_ptr(ptr) { minitl::addweak(m_ptr); }
    template< typename U >
    weak(ref<U> other) : m_ptr(other.operator->()) { minitl::addweak(m_ptr); }
    template< typename U >
    weak(const scoped<U>& other) : m_ptr(other.operator->()) { minitl::addweak(m_ptr); }
    weak(const weak& other) : m_ptr(other.operator->()) { minitl::addweak(m_ptr); }
    template< typename U >
    weak(const weak<U>& other) : m_ptr(other.operator->()) { minitl::addweak(m_ptr); }
    ~weak() { minitl::decweak(m_ptr); }

    weak& operator=(const weak& other) { weak(other).swap(*this); return *this; }
    template< typename U >
    weak& operator=(const weak<U>& other) { weak(other).swap(*this); return *this; }
    template< typename U >
    weak& operator=(U* other) { weak(other).swap(*this); return *this; }

    T* operator->() const { return m_ptr; }
    operator const void*() const { return m_ptr; }
    bool operator!() const { return m_ptr == 0; }

    void clear() { minitl::decweak(m_ptr); m_ptr = 0; }
};

}

/*****************************************************************************/
#endif
