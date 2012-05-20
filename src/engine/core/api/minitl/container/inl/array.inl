/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_ARRAY_INL_
#define BE_MINITL_CONTAINER_INL_ARRAY_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>
#include    <minitl/container/array.hh>

namespace minitl
{

template< typename T >
array<T>::array(BugEngine::Allocator& allocator, u32 size)
    :   m_array(new(allocator) T[size])
    ,   m_size(size)
{
}

template< typename T >
array<T>::~array()
{
    ::operator delete[](allocator, m_array);
}

template< typename T >
T* array<T>::begin()
{
    return m_array;
}

template< typename T >
T* array<T>::end()
{
    return m_array+m_size;
}

template< typename T >
const T* array<T>::begin() const
{
    return m_array;
}

template< typename T >
const T* array<T>::end() const
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
