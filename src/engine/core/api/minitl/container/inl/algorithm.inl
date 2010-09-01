/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
#define BE_MINITL_CONTAINER_INL_ALGORITHM_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>


namespace minitl
{

template< typename ITERATOR, typename FUNCTOR >
void for_each(ITERATOR first, ITERATOR last, FUNCTOR f)
{
    for(; first != last; ++first) f(*first);
}

template< typename T >
T* advance(T* input, ptrdiff_t offset)
{
    char *ptr = reinterpret_cast<char*>(input);
    ptr = ptr + be_align(sizeof(T),be_alignof(T))*offset;
    return reinterpret_cast<T*>(ptr);
}

template< typename T >
const T* advance(const T* input, ptrdiff_t offset)
{
    const char *ptr = reinterpret_cast<const char*>(input);
    ptr = ptr + be_align(sizeof(T),be_alignof(T))*offset;
    return reinterpret_cast<const T*>(ptr);
}


}

/*****************************************************************************/
#endif
