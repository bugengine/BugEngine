/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_ALGORITHM_HH_
#define BE_MINITL_CONTAINER_ALGORITHM_HH_
/*****************************************************************************/

namespace minitl
{

template< typename ITERATOR, typename FUNCTOR >
void for_each(ITERATOR first, iterator last, FUNCTOR f);

}

#include <minitl/container/inl/algorithm.inl>

/*****************************************************************************/
#endif
