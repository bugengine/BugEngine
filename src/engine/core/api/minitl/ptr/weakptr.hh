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
    const pointer* m_ptr;
private:
    void swap(weak& other)
    {
        const minitl::pointer* tmp = other.m_ptr;
        other.m_ptr = m_ptr;
        m_ptr = tmp;
    }
public:
    weak() : m_ptr(0) {}
    weak(T* ptr) : m_ptr(ptr) { if (m_ptr) m_ptr->addweak(); }
    template< typename U >
    weak(ref<U> other) : m_ptr(checkIsA<T>(other.operator->())) { if (m_ptr) m_ptr->addweak(); }
    template< typename U >
    weak(const scoped<U>& other) : m_ptr(checkIsA<T>(other.operator->())) { if (m_ptr) m_ptr->addweak(); }
    weak(const weak& other) : m_ptr(other.operator->()) { if (m_ptr) m_ptr->addweak(); }
    template< typename U >
    weak(const weak<U>& other) : m_ptr(checkIsA<T>(other.operator->())) { if (m_ptr) m_ptr->addweak(); }
    ~weak() { if (m_ptr) m_ptr->decweak(); }

    weak& operator=(const weak& other) { weak(other).swap(*this); return *this; }
    template< typename U >
    weak& operator=(const weak<U>& other) { weak(other).swap(*this); return *this; }
    template< typename U >
    weak& operator=(U* other) { weak(other).swap(*this); return *this; }

    T* operator->() const { return static_cast<T*>(const_cast<minitl::pointer*>(m_ptr)); }
    operator const void*() const { return m_ptr; }
    bool operator!() const { return m_ptr == 0; }
    T& operator*() { return *static_cast<T*>(const_cast<minitl::pointer*>(m_ptr)); }

    void clear() { if (m_ptr) m_ptr->decweak(); m_ptr = 0; }
};

}

/*****************************************************************************/
#endif
