/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_ITERATORS_INL_
#define BE_MINITL_CONTAINER_INL_ITERATORS_INL_
/**************************************************************************************************/
#include <bugengine/minitl/assert.hh>

namespace minitl {

namespace impl {

template < typename ITERATOR, typename ITERATOR_TYPE >
static typename iterator_traits< ITERATOR >::difference_type
distance(const ITERATOR& t1, const ITERATOR& t2, ITERATOR_TYPE /*type*/)
{
    typename ITERATOR::difference_type result = 0;
    while(t1 != t2)
    {
        result++;
        ++t1;
    }
    return result;
}

template < typename ITERATOR >
static typename iterator_traits< ITERATOR >::difference_type
distance(const ITERATOR& t1, const ITERATOR& t2, random_access_iterator_tag /*type*/)
{
    return t2 - t1;
}

}  // namespace impl

template < typename ITERATOR >
typename iterator_traits< ITERATOR >::iterator_category iterator_category(const ITERATOR& /*it*/)
{
    return typename iterator_traits< ITERATOR >::iterator_category();
}

template < typename ITERATOR >
typename iterator_traits< ITERATOR >::difference_type distance(const ITERATOR& first,
                                                               const ITERATOR& last)
{
    return impl::distance(first, last, iterator_category(first));
}

template < typename T >
difference_type distance(T* t1, T* t2)
{
    const byte* ptr1 = reinterpret_cast< const byte* >(t1);
    const byte* ptr2 = reinterpret_cast< const byte* >(t2);
    ptrdiff_t   d    = ptr2 - ptr1;
    be_assert(d % minitl::align(sizeof(T), be_alignof(T)) == 0,
              "distance between %p and %p is not a multiple of the size" | t1 | t2);
    return d / minitl::align(sizeof(T), be_alignof(T));
}

template < typename T >
difference_type distance(const T* t1, const T* t2)
{
    const byte* ptr1 = reinterpret_cast< const byte* >(t1);
    const byte* ptr2 = reinterpret_cast< const byte* >(t2);
    ptrdiff_t   d    = ptr2 - ptr1;
    be_assert(d % align(sizeof(T), be_alignof(T)) == 0,
              "distance between %p and %p is not a multiple of the size" | t1 | t2);
    return d / align(sizeof(T), be_alignof(T));
}

template < typename ITERATOR >
static ITERATOR advance(const ITERATOR&                                       it,
                        typename iterator_traits< ITERATOR >::difference_type offset)
{
    return it + offset;
}

template < typename T >
static T* advance_ptr(T* input, difference_type offset)
{
    char* ptr = reinterpret_cast< char* >(input);
    ptr       = ptr + minitl::align(sizeof(T), be_alignof(T)) * offset;
    return reinterpret_cast< T* >(ptr);
}

template < typename T >
static const T* advance_ptr(const T* input, difference_type offset)
{
    const char* ptr = reinterpret_cast< const char* >(input);
    ptr             = ptr + minitl::align(sizeof(T), be_alignof(T)) * offset;
    return reinterpret_cast< const T* >(ptr);
}

}  // namespace minitl

/**************************************************************************************************/
#endif
