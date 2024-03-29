/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_SWAP_INL_
#define BE_MINITL_CONTAINER_INL_SWAP_INL_
/**************************************************************************************************/
#include <bugengine/minitl/swap.hh>

namespace minitl {

template < typename T >
void swap(T& a, T& b)
{
    T c = a;
    a   = b;
    b   = c;
}

}  // namespace minitl

/**************************************************************************************************/
#endif
