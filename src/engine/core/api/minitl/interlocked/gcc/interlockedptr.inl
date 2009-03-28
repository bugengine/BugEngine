/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_MINITL_INTERLOCKED_WIN32_INTERLOCKEDPTR_INL_
#define BE_MINITL_INTERLOCKED_WIN32_INTERLOCKEDPTR_INL_
/*****************************************************************************/
#ifdef _X64
# include    <intrin.h>
# pragma intrinsic(_InterlockedExchangePointer)
# pragma intrinsic(_InterlockedCompareExchangePointer)
# define _BEInterlockedExchangePointer(p, v)              (void*)_InterlockedExchange((LONG*)(p), (LONG)(v))
# define _BEInterlockedCompareExchangePointer(p, v, cmp)  (void*)_InterlockedCompareExchange((LONG*)(p), (LONG)(v), (LONG)(cmp))
#else
# pragma intrinsic(_InterlockedExchange)
# pragma intrinsic(_InterlockedCompareExchange)
# define _BEInterlockedExchangePointer(p, v)              (void*)_InterlockedExchange((LONG*)(p), (LONG)(v))
# define _BEInterlockedCompareExchangePointer(p, v, cmp)  (void*)_InterlockedCompareExchange((LONG*)(p), (LONG)(v), (LONG)(cmp))
#endif

namespace minitl
{

template< typename T >
iptr<T>::iptr()
{
}

template< typename T >
iptr<T>::iptr(T* value)
    :   m_value(value)
{
}

template< typename T >
template< typename U >
iptr<T>::iptr(const iptr<U>& other)
:   m_value(static_cast<T*>(other.m_value))
{
}

template< typename T >
template< typename U >
iptr<T>& iptr<T>::operator=(U* other)
{
    exchange(static_cast<T*>(other));
    return *this;
}

template< typename T >
template< typename U >
iptr<T>& iptr<T>::operator=(const iptr<U>& other)
{
    exchange(static_cast<T*>(other.m_value));
    return *this;
}

template< typename T >
iptr<T>::operator T*()
{
    return m_value;
}

template< typename T >
iptr<T>::operator const T*() const
{
    return m_value;
}

template< typename T >
T* iptr<T>::exchange(T* value)
{
    return (T*)_BEInterlockedExchangePointer((void* volatile *)&m_value, value);
}

template< typename T >
T* iptr<T>::CAS(T* value, T* compare)
{
    return (T*)_BEInterlockedCompareExchangePointer((void* volatile *)&m_value, value, compare);
}

template< typename T >
inline bool operator==(const iptr<T>& p1, const iptr<T>& p2)
{
    return ((T*)p1) == ((T*)p2);
}

}

/*****************************************************************************/
#endif
