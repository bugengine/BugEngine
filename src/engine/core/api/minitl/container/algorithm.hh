/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_ALGORITHM_HH_
#define BE_MINITL_CONTAINER_ALGORITHM_HH_
/*****************************************************************************/

namespace minitl
{

typedef size_t      size_type;
typedef ptrdiff_t   difference_type;

template< typename ITERATOR, typename FUNCTOR >
void for_each(ITERATOR first, ITERATOR last, FUNCTOR f);

template< typename T >
T* advance(T* input, difference_type offset);
template< typename T >
const T* advance(const T* input, difference_type offset);


template< typename T >
difference_type distance(const T* t1, const T* t2)
{
    const byte *ptr1 = reinterpret_cast<const byte*>(t1);
    const byte *ptr2 = reinterpret_cast<const byte*>(t2);
    ptrdiff_t d = ptr2 - ptr1;
    be_assert(d %  be_align(sizeof(T),be_alignof(T)) == 0, "distance between %p and %p is not a multiple of the size" | t1 | t2);
    return d / be_align(sizeof(T),be_alignof(T));
}

}

#include <minitl/container/inl/algorithm.inl>

/*****************************************************************************/
#endif
