/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
#define BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>


namespace minitl
{

namespace _
{

template< typename ITERATOR, typename COMPARE >
struct SortPredicate
{
    ITERATOR    m_iterator;
    COMPARE     m_compare;
    SortPredicate(ITERATOR it)
        :   m_iterator(it)
    {
    }
    bool operator()(const typename ITERATOR::value_type& r) { return m_compare(*m_iterator, r); }
};

}


template< typename T >
void swap(T& a, T& b)
{
    T c = a; a = b; b = c;
}

template< typename ITERATOR, typename FUNCTOR >
void for_each(ITERATOR first, ITERATOR last, FUNCTOR f)
{
    for(; first != last; ++first) f(*first);
}

template< typename ITERATOR, typename PREDICATE >
ITERATOR partition(ITERATOR first, ITERATOR last, PREDICATE p)
{
    ITERATOR middle = first;
    for( ; first != last; ++first)
    {
        if(!p(*first))
        {
            minitl::swap(*first, *middle);
            ++middle;
        }
    }
    return middle;
}

template< typename ITERATOR, typename COMPARE >
void sort(ITERATOR first, ITERATOR last, COMPARE s)
{
    typename ITERATOR::difference_type d = distance(first, last);
    if(d > 1)
    {
        ITERATOR reallast = last - 1;
        ITERATOR t = first + d/2;
        swap(*t, *reallast);
        t = partition(first, reallast, _::SortPredicate<ITERATOR, COMPARE>(reallast));
        swap(*t, *reallast);
        sort(first, t, s);
        sort(t, last, s);
    }
}

}

/*****************************************************************************/
#endif
