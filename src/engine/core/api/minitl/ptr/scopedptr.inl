/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_SCOPEDPTR_INL_
#define BE_MINITL_PTR_SCOPEDPTR_INL_
/*****************************************************************************/
#include   <typeinfo>

namespace minitl
{

template< typename T >
scoped<T>::scoped(T* value, Deleter* deleter)
:   m_ptr(value)
{
    be_assert(value->m_deleter == 0, "value of type %s already has a deleter; being refcounting multiple times?" | typeid(T).name());
    value->m_deleter = deleter;
}

template< typename T >
scoped<T>::scoped()
:   m_ptr(0)
{
}

template< typename T >
scoped<T>::~scoped()
{
    checked_delete<T>(m_ptr);
}

template< typename T >
scoped<T>::scoped(const scoped& other)
:   m_ptr(other.m_ptr)
{
    other.m_ptr = 0;
}

template< typename T >
template< typename U >
scoped<T>::scoped(const scoped<U>& other)
:   m_ptr(other.m_ptr)
{
    other.m_ptr = 0;
}

template< typename T >
scoped<T>& scoped<T>::operator=(const scoped& other)
{
    checked_delete<T>(m_ptr);
    m_ptr = other.m_ptr;
    other.m_ptr = 0;
    return *this;
}


template< typename T >
T* scoped<T>::operator->() const
{
    return m_ptr;
}

template< typename T >
scoped<T>::operator const void*() const
{
    return m_ptr;
}

template< typename T >
bool scoped<T>::operator!() const
{
    return m_ptr == 0;
}

template< typename T >
T& scoped<T>::operator*()
{
    return *m_ptr;
}

template<typename T, typename U>
bool operator==(const scoped<T>& ref1, const scoped<U>& ref2)
{
    return ref1.operator->() == ref2.operator->();
}

template<typename T, typename U>
bool operator!=(const scoped<T>& ref1, const scoped<U>& ref2)
{
    return ref1.operator->() != ref2.operator->();
}

}

/*****************************************************************************/
#endif
