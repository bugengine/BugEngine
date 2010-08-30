/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
#define BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>


namespace minitl
{

template< typename ITERATOR, typename FUNCTOR >
void for_each(ITERATOR first, iterator last, FUNCTOR f)
{
    for(; first != last; ++first) f(*first);
}

}

/*****************************************************************************/
#endif
