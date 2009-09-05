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

#ifndef BE_MINITL_MEMORY_POOL_INL_
#define BE_MINITL_MEMORY_POOL_INL_
/*****************************************************************************/
#include    <core/memory/malloc.hh>

namespace minitl
{

template< typename T >
pool<T>::pool(size_t capacity, size_t alignment)
:   m_pool(BugEngine::Malloc::internalAllocArray<T>(capacity, alignment))
,   m_end((char*)m_pool + be_align(sizeof(T),alignment)*capacity)
{
    const size_t aligned_size = be_align(sizeof(T),alignment);
    for(size_t i = 0; i < capacity-1; ++i)
        m_items.push((inode*)((char*)m_pool+i*aligned_size));
}

template< typename T >
pool<T>::~pool()
{
    BugEngine::Malloc::internalFree(m_pool);
}

template< typename T >
T* pool<T>::allocate()
{
    T* result = (T*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return result;
}

template< typename T >
void pool<T>::release(T* t)
{
    be_assert(t >= m_pool && t < m_end, "releasing a node that is outside the node range");
    m_items.push((inode*)t);
}

}

/*****************************************************************************/
#endif

