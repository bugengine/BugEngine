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
    :   m_refCount(new(allocator) i_u32(i_u32::Zero))
    ,   m_array(new(allocator) T[size])
    ,   m_allocator(allocator)
    ,   m_size(size)
{
    ++ (*m_refCount);
}

template< typename T >
template< typename ITERATOR >
array<T>::array(Allocator& allocator, ITERATOR begin, ITERATOR end)
    :   m_refCount(new(allocator) i_u32(i_u32::Zero))
    ,   m_array(new(allocator) T[distance(begin, end)])
    ,   m_allocator(allocator)
    ,   m_size(distance(begin, end))
{
    ++ (*m_refCount);
    for (int i = 0; begin != end; ++begin, ++i)
    {
        m_array[i] = *begin;
    }
}

template< typename T >
array<T>::array(const array<T>& rhs)
    :   m_refCount(rhs.m_refCount)
    ,   m_array(rhs.m_array)
    ,   m_allocator(rhs.m_allocator)
    ,   m_size(rhs.m_size)
{
    ++ (*m_refCount);
}

template< typename T >
array<T>::~array()
{
    if (0 == (*m_refCount)--)
    {
        ::operator delete[](m_array, m_allocator);
    }
}

template< typename T >
typename array<T>::iterator array<T>::begin()
{
    return m_array;
}

template< typename T >
typename array<T>::iterator array<T>::end()
{
    return m_array+m_size;
}

template< typename T >
typename array<T>::const_iterator array<T>::begin() const
{
    return m_array;
}

template< typename T >
typename array<T>::const_iterator array<T>::end() const
{
    return m_array+m_size;
}

template< typename T >
T& array<T>::operator[](u32 index)
{
    be_assert(index < m_size, "index %d out of bounds: array size is %d"|index|m_size);
    return m_array[index];
}

template< typename T >
const T& array<T>::operator[](u32 index) const
{
    be_assert(index < m_size, "index %d out of bounds: array size is %d"|index|m_size);
    return m_array[index];
}

}

/*****************************************************************************/
#endif
