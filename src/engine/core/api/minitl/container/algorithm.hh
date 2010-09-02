/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_ALGORITHM_HH_
#define BE_MINITL_CONTAINER_ALGORITHM_HH_
/*****************************************************************************/

namespace minitl
{


template< typename T >
struct less
{
    bool operator()(const T& a, const T& b) const
    {
        return a < b;
    }
};

template< typename T >
void swap(T& a, T& b);

template< typename ITERATOR, typename FUNCTOR >
void for_each(ITERATOR first, ITERATOR last, FUNCTOR f);

template< typename ITERATOR, typename PREDICATE >
ITERATOR partition(ITERATOR first, ITERATOR last, PREDICATE p);

template< typename ITERATOR, typename COMPARE >
void sort(ITERATOR first, ITERATOR last, COMPARE f);

}

#include <minitl/container/inl/algorithm.inl>

/*****************************************************************************/
#endif
