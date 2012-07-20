/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_PAIR_INL_
#define BE_MINITL_CONTAINER_PAIR_INL_
/*****************************************************************************/

namespace minitl
{

template< typename T1, typename T2 >
pair<T1, T2>::pair()
    :   first()
    ,   second()
{
}

template< typename T1, typename T2 >
pair<T1, T2>::pair(const T1& t1, const T2& t2)
    :   first(t1)
    ,   second(t2)
{
}

template< typename T1, typename T2 >
template< typename U1, typename U2 >
pair<T1, T2>::pair(const pair<U1, U2>& p)
    :   first(static_cast<U1>(p.first))
    ,   second(static_cast<U2>(p.second))
{
}

template< typename T1, typename T2 >
static inline pair<T1,T2> make_pair(const T1& t1, const T2& t2)
{
    return pair<T1, T2>(t1, t2);
}

}

/*****************************************************************************/
#endif
