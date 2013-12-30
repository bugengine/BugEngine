/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INL_TUPLE_INL_
#define BE_MINITL_INL_TUPLE_INL_
/*****************************************************************************/

namespace minitl
{

template< typename T1, typename T2 >
static inline tuple<T1,T2> make_tuple(const T1& t1, const T2& t2)
{
    return tuple<T1, T2>(t1, t2);
}

template< typename T1, typename T2, typename T3 >
static inline tuple<T1,T2,T3> make_tuple(const T1& t1, const T2& t2, const T3& t3)
{
    return tuple<T1, T2, T3>(t1, t2, t3);
}

template< typename T1, typename T2, typename T3, typename T4 >
static inline tuple<T1,T2,T3,T4> make_tuple(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
    return tuple<T1, T2, T3, T4>(t1, t2, t3, t4);
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
static inline tuple<T1,T2,T3,T4,T5> make_tuple(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
    return tuple<T1, T2, T3, T4, T5>(t1, t2, t3, t4, t5);
}

}

/*****************************************************************************/
#endif
