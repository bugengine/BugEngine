/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_ITERATORS_HH_
#define BE_MINITL_ITERATORS_HH_
/*****************************************************************************/
#include <stddef.h>

//! \addtogroup minitl
//! @{
namespace minitl
{

typedef size_t      size_type;
typedef ptrdiff_t   difference_type;

struct input_iterator_tag                                               { };
struct output_iterator_tag                                              { };
struct forward_iterator_tag : public input_iterator_tag                 { };
struct bidirectional_iterator_tag : public forward_iterator_tag         { };
struct random_access_iterator_tag : public bidirectional_iterator_tag   { };

template< typename T, typename DIFF = ::minitl::difference_type >
struct input_iterator
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct output_iterator
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct forward_iterator : public input_iterator<T,DIFF>
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct bidirectional_iterator : public forward_iterator<T,DIFF>
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct random_access_iterator : public bidirectional_iterator<T,DIFF>
{
};



template< typename ITERATOR >
struct iterator_traits
{
    typedef typename ITERATOR::iterator_category    iterator_category;
    typedef typename ITERATOR::value_type           value_type;
    typedef typename ITERATOR::pointer              pointer;
    typedef typename ITERATOR::reference            reference;
    typedef typename ITERATOR::size_type            size_type;
    typedef typename ITERATOR::difference_type      difference_type;
};
template< typename T >
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef minitl::size_type           size_type;
    typedef minitl::difference_type     difference_type;
};
template< typename T >
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef const T                     value_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
    typedef minitl::size_type           size_type;
    typedef minitl::difference_type     difference_type;
};

template< typename ITERATOR >
static typename iterator_traits<ITERATOR>::iterator_category iterator_category(const ITERATOR& it);

template< typename ITERATOR >
static typename iterator_traits<ITERATOR>::difference_type distance(const ITERATOR& first, const ITERATOR& last);

template< typename ITERATOR >
static ITERATOR advance(const ITERATOR& it, typename iterator_traits<ITERATOR>::difference_type offset);

}
//! @}

#include    <minitl/inl/iterator.inl>

/*****************************************************************************/
#endif
