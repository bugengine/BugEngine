/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_RANGE_SEQUENCE_INL_
#define BE_SCHEDULER_RANGE_SEQUENCE_INL_
/*****************************************************************************/

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
range_sequence<T> range_sequence<T>::split()
{
    T oldbegin = m_begin;
    m_begin = m_begin + (m_end-m_begin)/2;
    return range_sequence<T>(oldbegin, m_begin);
}

}}

/*****************************************************************************/
#endif
