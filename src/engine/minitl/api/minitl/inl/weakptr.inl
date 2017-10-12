/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_INL_WEAKPTR_INL_
#define BE_MINITL_INL_WEAKPTR_INL_
/**************************************************************************************************/
#include    <minitl/stdafx.h>
#include    <minitl/refcountable.hh>
#include    <minitl/scopedptr.hh>
#include    <minitl/refptr.hh>
#include    <minitl/hash.hh>


namespace minitl
{

template< typename T >
void weak<T>::swap(weak& other)
{
    pointer tmp = other.m_ptr;
    other.m_ptr = m_ptr;
    m_ptr = tmp;
}


template< typename T >
weak<T>::weak()
    :   m_ptr(0)
{
}

template< typename T >
weak<T>::weak(T* p)
    :   m_ptr(p)
{
#if BE_ENABLE_WEAKCHECK
    if (m_ptr) m_ptr->addweak();
#endif
}

template< typename T >
template< typename U >
weak<T>::weak(ref<U> other)
    :   m_ptr(checkIsA<T>(other.operator->()))
{
#if BE_ENABLE_WEAKCHECK
    if (m_ptr) m_ptr->addweak();
#endif
}

template< typename T >
template< typename U >
weak<T>::weak(const scoped<U>& other)
    :   m_ptr(checkIsA<T>(other.operator->()))
{
#if BE_ENABLE_WEAKCHECK
    if (m_ptr) m_ptr->addweak();
#endif
}

template< typename T >
weak<T>::weak(const weak& other)
    :   m_ptr(other.operator->())
{
#if BE_ENABLE_WEAKCHECK
    if (m_ptr) m_ptr->addweak();
#endif
}

template< typename T >
template< typename U >
weak<T>::weak(const weak<U>& other)
    : m_ptr(checkIsA<T>(other.operator->()))
{
#if BE_ENABLE_WEAKCHECK
    if (m_ptr) m_ptr->addweak();
#endif
}

template< typename T >
weak<T>::~weak()
{
#if BE_ENABLE_WEAKCHECK
    if (m_ptr) m_ptr->decweak();
#endif
}

template< typename T >
weak<T>& weak<T>::operator=(const weak& other)
{
    weak(other).swap(*this); return *this;
}

template< typename T >
template< typename U >
weak<T>& weak<T>::operator=(const weak<U>& other)
{
    weak(other).swap(*this);
    return *this;
}

template< typename T >
template< typename U >
weak<T>& weak<T>::operator=(U* other)
{
    weak(other).swap(*this);
    return *this;
}

template< typename T >
T* weak<T>::operator->() const
{
    return static_cast<T*>(m_ptr);
}

template< typename T >
weak<T>::operator const void*() const
{
    return m_ptr;
}

template< typename T >
bool weak<T>::operator!() const
{
    return m_ptr == 0;
}

template< typename T >
T& weak<T>::operator*()
{
    return *static_cast<T*>(m_ptr);
}

template< typename T >
void weak<T>::clear()
{
#if BE_ENABLE_WEAKCHECK
    if (m_ptr) m_ptr->decweak();
#endif
    m_ptr = 0;
}

}

/**************************************************************************************************/
#endif
