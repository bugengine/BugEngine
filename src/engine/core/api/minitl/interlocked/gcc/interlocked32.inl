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

#ifndef BE_MINITL_INTERLOCKED_WIN32_INTERLOCKED32_INL_
#define BE_MINITL_INTERLOCKED_WIN32_INTERLOCKED32_INL_
/*****************************************************************************/
#include    <minitl/type/cast>
#include    <intrin.h>
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedXor)

namespace minitl
{

template< typename T >
interlocked32<T>::interlocked32()
{
}

template< typename T >
interlocked32<T>::interlocked32(T value)
:   m_value(LONG(value))
{
}

template< typename T >
interlocked32<T>::~interlocked32()
{
}

template< typename T >
interlocked32<T>::operator T() const
{
    return checked_numcast<T>(_InterlockedExchangeAdd(const_cast<volatile long*>(&m_value), 0));
}

template< typename T >
T interlocked32<T>::operator= (T value)
{
    _InterlockedExchange(&m_value, LONG(value));
    return value;
}

template< typename T >
T interlocked32<T>::operator++()
{
    return checked_numcast<T>(_InterlockedIncrement(&m_value));
}

template< typename T >
T interlocked32<T>::operator++(int)
{
    return checked_numcast<T>(_InterlockedExchangeAdd(&m_value, 1));
}

template< typename T >
T interlocked32<T>::operator--()
{
    return checked_numcast<T>(_InterlockedDecrement(&m_value));
}

template< typename T >
T interlocked32<T>::operator--(int)
{
    return checked_numcast<T>(_InterlockedExchangeAdd(&m_value, -1));
}

template< typename T >
T interlocked32<T>::operator+=(T value)
{
    return checked_numcast<T>(_InterlockedExchangeAdd(&m_value, value)+value);
}

template< typename T >
T interlocked32<T>::operator-=(T value)
{
    return checked_numcast<T>(_InterlockedExchangeAdd(&m_value, -(long)value)-value);
}

template< typename T >
T interlocked32<T>::operator|=(T value)
{
    return checked_numcast<T>(_InterlockedOr(&m_value, value));
}

template< typename T >
T interlocked32<T>::operator&=(T value)
{
    return checked_numcast<T>(_InterlockedAnd(&m_value, -value));
}

template< typename T >
T interlocked32<T>::operator^=(T value)
{
    return checked_numcast<T>(_InterlockedXor(&m_value, value));
}

template< typename T >
T interlocked32<T>::addExchange(T value)
{
    return checked_numcast<T>(_InterlockedExchangeAdd(&m_value, value));
}

template< typename T >
T interlocked32<T>::exchange(T value)
{
    return checked_numcast<T>(_InterlockedExchange(&m_value, value));
}

template< typename T >
T interlocked32<T>::CAS(T value, T compare)
{
    return checked_numcast<T>(_InterlockedCompareExchange(&m_value, value, compare));
}

}

/*****************************************************************************/
#endif
