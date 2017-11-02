/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
#define BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
/**************************************************************************************************/
#include    <minitl/iterator.hh>

namespace minitl
{

namespace impl
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
    bool operator()(const ITERATOR& r) { return m_compare(*m_iterator, *r); }
};

}


template< typename ITERATOR, typename FUNCTOR >
void for_each(ITERATOR first, ITERATOR last, FUNCTOR f)
{
    for (; first != last; ++first) f(*first);
}


template< typename ITERATOR, typename T >
void fill(ITERATOR first, ITERATOR last, const T& value)
{
    for (; first != last; ++first) *first = value;
}


template< typename ITERATOR, typename PREDICATE >
ITERATOR partition(ITERATOR first, ITERATOR last, PREDICATE p)
{
    ITERATOR middle = first;
    for ( ; first != last; ++first)
    {
        if (!p(first))
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
    minitl::difference_type d = distance(first, last);
    if (d > 1)
    {
        ITERATOR reallast = last - 1;
        ITERATOR t = first + d/2;
        swap(*t, *reallast);
        t = partition(first, reallast, impl::SortPredicate<ITERATOR, COMPARE>(reallast));
        swap(*t, *reallast);
        sort(first, t, s);
        sort(t, last, s);
    }
}


template< typename ITERATOR, typename T >
ITERATOR find(const T& t, ITERATOR begin, ITERATOR end)
{
    for (ITERATOR it = begin; it != end; ++it)
    {
        if (*it == t)
            return it;
    }
    return end;
}


}

/**************************************************************************************************/
#endif
