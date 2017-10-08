/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_WEAKPTR_HH_
#define BE_MINITL_WEAKPTR_HH_
/**************************************************************************************************/
#include    <minitl/stdafx.h>
#include    <minitl/refcountable.hh>
#include    <minitl/scopedptr.hh>
#include    <minitl/refptr.hh>
#include    <minitl/hash.hh>


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
    template< typename OBJECT >
    struct ptr
    {
        typedef minitl::pointer* pointer;
    };
    template< typename OBJECT >
    struct ptr<const OBJECT>
    {
        typedef const minitl::pointer* pointer;
    };
private:
    typename ptr<T>::pointer m_ptr;
private:
    void swap(weak& other)
    {
        typename ptr<T>::pointer tmp = other.m_ptr;
        other.m_ptr = m_ptr;
        m_ptr = tmp;
    }
public:
    weak() : m_ptr(0) {}
    weak(T* p)
        : m_ptr(p)
    {
        #if BE_ENABLE_WEAKCHECK
            if (m_ptr) m_ptr->addweak();
        #endif
    }
    template< typename U >
    weak(ref<U> other)
        : m_ptr(checkIsA<T>(other.operator->()))
    {
        #if BE_ENABLE_WEAKCHECK
            if (m_ptr) m_ptr->addweak();
        #endif
    }
    template< typename U >
    weak(const scoped<U>& other)
        : m_ptr(checkIsA<T>(other.operator->()))
    {
        #if BE_ENABLE_WEAKCHECK
            if (m_ptr) m_ptr->addweak();
        #endif
    }
    weak(const weak& other)
        : m_ptr(other.operator->())
    {
        #if BE_ENABLE_WEAKCHECK
            if (m_ptr) m_ptr->addweak();
        #endif
    }
    template< typename U >
    weak(const weak<U>& other)
        : m_ptr(checkIsA<T>(other.operator->()))
    {
        #if BE_ENABLE_WEAKCHECK
            if (m_ptr) m_ptr->addweak();
        #endif
    }
    ~weak()
    {
        #if BE_ENABLE_WEAKCHECK
            if (m_ptr) m_ptr->decweak();
        #endif
    }

    weak& operator=(const weak& other) { weak(other).swap(*this); return *this; }
    template< typename U >
    weak& operator=(const weak<U>& other) { weak(other).swap(*this); return *this; }
    template< typename U >
    weak& operator=(U* other) { weak(other).swap(*this); return *this; }

    T* operator->() const { return static_cast<T*>(m_ptr); }
    operator const void*() const { return m_ptr; }
    bool operator!() const { return m_ptr == 0; }
    T& operator*() { return *static_cast<T*>(m_ptr); }

    void clear()
    {
        #if BE_ENABLE_WEAKCHECK
            if (m_ptr) m_ptr->decweak();
        #endif
        m_ptr = 0;
    }
};

template< typename T >
struct hash< weak<T> >
{
    u32 operator()(weak<T> t)
    {
        return hash<T*>()(t.operator->());
    }
    bool operator()(weak<T> t1, weak<T> t2)
    {
        return hash<T*>()(t1.operator->(), t2.operator ->());
    }
};

template< u16 SIZE > class format;
template< typename T, u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& format, weak<T> t)
{
    return format | t.operator->();
}

}

#include <minitl/inl/weakptr.inl>

/**************************************************************************************************/
#endif
