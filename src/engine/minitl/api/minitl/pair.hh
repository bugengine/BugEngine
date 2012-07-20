/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PAIR_HH_
#define BE_MINITL_PAIR_HH_
/*****************************************************************************/

namespace minitl
{

template< typename T1, typename T2 >
struct pair
{
    T1 first;
    T2 second;
    inline pair();
    inline pair(const T1& t1, const T2& t2);
    template< typename U1, typename U2 > inline pair(const pair<U1, U2>& p);
};

template< typename T1, typename T2 >
static inline pair<T1,T2> make_pair(const T1& t1, const T2& t2);

}

#include <minitl/inl/pair.inl>

/*****************************************************************************/
#endif
