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

#ifndef BE_MINITL_INTERLOCKED_WIN32_INTERLOCKED64_INL_
#define BE_MINITL_INTERLOCKED_WIN32_INTERLOCKED64_INL_
/*****************************************************************************/
#include    <minitl/type/cast>
#include    <intrin.h>

namespace minitl
{

template< typename T >
interlocked64<T>::interlocked64()
{
}

template< typename T >
interlocked64<T>::interlocked64(T value)
:   m_value(LONG64(value))
{
}

template< typename T >
interlocked64<T>::~interlocked64()
{
}

template< typename T >
interlocked64<T>::operator T() const
{
    return checked_numcast<T>(InterlockedAdd64(&m_value, 0));
}

template< typename T >
T interlocked64<T>::operator= (T value)
{
    return checked_numcast<T>(InterlockedExchange64(&m_value, LONG64(value)));
}

template< typename T >
T interlocked64<T>::operator++()
{
    return checked_numcast<T>(InterlockedIncrement64(&m_value));
}

template< typename T >
T interlocked64<T>::operator++(int)
{
    return checked_numcast<T>(InterlockedExchangeAdd64(&m_value, 1));
}

template< typename T >
T interlocked64<T>::operator--()
{
    return checked_numcast<T>(InterlockedDecrement64(&m_value));
}

template< typename T >
T interlocked64<T>::operator--(int)
{
    return checked_numcast<T>(InterlockedExchangeAdd64(&m_value, -1));
}

template< typename T >
T interlocked64<T>::operator+=(T value)
{
    return checked_numcast<T>(InterlockedAdd64(&m_value, value));
}

template< typename T >
T interlocked64<T>::operator-=(T value)
{
    return checked_numcast<T>(InterlockedAdd64(&m_value, -value));
}

template< typename T >
T interlocked64<T>::operator|=(T value)
{
    return checked_numcast<T>(InterlockedOr64(&m_value, value));
}

template< typename T >
T interlocked64<T>::operator&=(T value)
{
    return checked_numcast<T>(InterlockedAnd64(&m_value, -value));
}

template< typename T >
T interlocked64<T>::operator^=(T value)
{
    return checked_numcast<T>(InterlockedXor64(&m_value, value));
}

template< typename T >
T interlocked64<T>::addExchange(T value)
{
    return checked_numcast<T>(_InterlockedExchangeAdd64(&m_value, value));
}

template< typename T >
T interlocked64<T>::exchange(T value)
{
    return checked_numcast<T>(_InterlockedExchange64(&m_value, value));
}

}

/*****************************************************************************/
#endif
