/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_ARRAY_HH_
#define BE_MINITL_CONTAINER_ARRAY_HH_
/*****************************************************************************/
#include    <minitl/container/iterator.hh>
#include    <minitl/interlocked/interlocked.hh>
#include    <core/memory/malloc.hh>

namespace minitl
{

template< typename T >
class array
{
private:
    i_u32*                  m_refCount;
    T*                      m_array;
    BugEngine::Allocator&   m_allocator;
    u32                     m_size;
public:
    typedef const T* const_iterator;
    typedef T* iterator;
public:
    inline array(BugEngine::Allocator& allocator, u32 size);
    template< typename ITERATOR >
    inline array(BugEngine::Allocator& allocator, ITERATOR begin, ITERATOR end);
    inline array(const array<T>& rhs);
    inline ~array();

    inline iterator begin();
    inline iterator end();
    inline const_iterator begin() const;
    inline const_iterator end() const;

    inline T& operator[](u32 index);
    inline const T& operator[](u32 index) const;
};

}

#include <minitl/container/inl/array.inl>

/*****************************************************************************/
#endif
