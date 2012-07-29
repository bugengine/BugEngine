/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INL_ARRAY_INL_
#define BE_MINITL_INL_ARRAY_INL_
/*****************************************************************************/
#include    <minitl/assert.hh>
#include    <minitl/array.hh>
#include    <minitl/algorithm.hh>

namespace minitl
{

template< typename T >
array<T>::array(Allocator& allocator, u32 size)
    :   m_array(allocator, size)
{
    for (u32 i = 0; i < size; ++i)
    {
        new (&m_array[i]) T;
    }
}

template< typename T >
template< typename ITERATOR >
array<T>::array(Allocator& allocator, ITERATOR begin, ITERATOR end)
    :   m_array(allocator, distance(begin, end))
{
    for (int i = 0; begin != end; ++begin, ++i)
    {
        new (&m_array[i])T(*begin);
    }
}

template< typename T >
array<T>::array(const array<T>& other)
    :   m_array(other.m_array.arena(), 0)
{
    m_array.swap(other.m_array);
}

template< typename T >
array<T>::~array()
{
    for (u64 i = m_array.count(); i > 0; --i)
    {
        m_array[i-1].~T();
    }
}

template< typename T >
typename array<T>::iterator array<T>::begin()
{
    return &m_array[0];
}

template< typename T >
typename array<T>::iterator array<T>::end()
{
    return &m_array[m_array.count()];
}

template< typename T >
typename array<T>::const_iterator array<T>::begin() const
{
    return &m_array[0];
}

template< typename T >
typename array<T>::const_iterator array<T>::end() const
{
    return &m_array[m_array.count()];
}

template< typename T >
T& array<T>::operator[](u32 index)
{
    be_assert(index < m_array.count(), "index %d out of bounds: array size is %d"|index|m_array.count());
    return m_array[index];
}

template< typename T >
const T& array<T>::operator[](u32 index) const
{
    be_assert(index < m_array.count(), "index %d out of bounds: array size is %d"|index|m_array.count());
    return m_array[index];
}

}

/*****************************************************************************/
#endif
