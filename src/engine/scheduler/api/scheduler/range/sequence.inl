/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_RANGE_SEQUENCE_INL_
#define BE_SCHEDULER_RANGE_SEQUENCE_INL_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/range/sequence.hh>

namespace BugEngine { namespace Task
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
u32 range_sequence<T>::partCount(u32 /*workerCount*/) const
{
    return (m_end - m_begin + m_grain - 1) / m_grain;
}

template< typename T >
range_sequence<T> range_sequence<T>::part(u32 index, u32 total) const
{
    T begin = m_begin + index * m_grain;
    T end = (index == total-1) ? m_end : begin + m_grain;
    return range_sequence<T>(begin, end);
}

}}

/**************************************************************************************************/
#endif
