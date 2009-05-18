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

#ifndef BE_CORE_THREAD_SCHEDULER_RANGE_SEQUENCE_INL_
#define BE_CORE_THREAD_SCHEDULER_RANGE_SEQUENCE_INL_
/*****************************************************************************/

namespace BugEngine
{

template< typename T >
range_sequence<T>::range_sequence(T begin, T end, size_t grain)
:   m_begin(begin)
,   m_end(end)
,   m_grain(grain)
{
}

template< typename T >
range_sequence<T>::~range_sequence()
{
}

template< typename T >
T& range_sequence<T>::begin()
{
    return m_begin;
}

template< typename T >
T& range_sequence<T>::end()
{
    return m_end;
}

template< typename T >
size_t range_sequence<T>::size() const
{
    return m_end-m_begin;
}

template< typename T >
bool range_sequence<T>::atomic() const
{
    return size() <= m_grain;
}

template< typename T >
range_sequence<T> range_sequence<T>::split()
{
    T oldbegin = m_begin;
    m_begin = m_begin + (m_end-m_begin)/2;
    return range_sequence<T>(oldbegin, m_begin);
}

}

/*****************************************************************************/
#endif
