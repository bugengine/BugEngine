/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_REFPTR_INL_
#define BE_MINITL_PTR_REFPTR_INL_
/*****************************************************************************/
#include   <typeinfo>

namespace minitl
{

template< typename T >
void ref<T>::swap(ref<T>& other)
{
    const minitl::refcountable* tmpPtr = other.m_ptr;
    other.m_ptr = m_ptr;
    m_ptr = tmpPtr; 
}


template< typename T >
ref<T>::ref(T* value, BugEngine::Allocator& deleter)
:   m_ptr(value)
{
    be_assert(value->m_allocator == 0, "value of type %s already has a deleter; being refcounting multiple times?" | typeid(T).name());
    value->m_allocator = &deleter;
    minitl::addref(value);
}

template< typename T >
ref<T>::ref()
:   m_ptr(0)
{
}

template< typename T >
ref<T>::ref(const ref& other)
:   m_ptr(other.m_ptr)
{
    addref(m_ptr);
}

template< typename T >
template< typename U >
ref<T>::ref(const ref<U> other)
:   m_ptr(other.operator->())
{
    addref(m_ptr);
}

template< typename T >
template< typename U >
ref<T>::ref(const scoped<U> other)
:   m_ptr(other.operator->())
{
    addref(m_ptr);
    other.m_ptr = 0;
}

template< typename T >
ref<T>& ref<T>::operator=(const ref<T>& other)
{
    ref(other).swap(*this); return *this;
}

template< typename T >
template< typename U >
ref<T>& ref<T>::operator=(const ref<U>& other)
{
    ref(other).swap(*this); return *this;
}

template< typename T >
ref<T>::~ref()
{
    decref(m_ptr);
}

template< typename T >
T* ref<T>::operator->() const
{
    return static_cast<T*>(const_cast<minitl::refcountable*>(m_ptr));
}

template< typename T >
ref<T>::operator const void*() const
{
    return m_ptr;
}

template< typename T >
bool ref<T>::operator!() const
{
    return m_ptr == 0;
}

template< typename T >
T& ref<T>::operator*()
{
    return *static_cast<T*>(const_cast<minitl::refcountable*>(m_ptr));
}

template< typename T >
void ref<T>::clear()
{
    decref(m_ptr);
    m_ptr = 0;
}

template<typename T, typename U>
bool operator==(const ref<T>& ref1, const ref<U>& ref2)
{
    return ref1.operator->() == ref2.operator->();
}

template<typename T, typename U>
bool operator!=(const ref<T>& ref1, const ref<U>& ref2)
{
    return ref1.operator->() != ref2.operator->();
}

}

/*****************************************************************************/
#endif
