/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_TUPLE_HH_
#define BE_MINITL_TUPLE_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>

namespace minitl {

template < typename T1, typename T2, typename T3 = void, typename T4 = void, typename T5 = void >
struct tuple;

template < typename T1, typename T2 >
struct tuple< T1, T2, void, void, void >
{
    T1 first;
    T2 second;
    inline tuple() : first(), second()
    {
    }
    inline tuple(const T1& t1, const T2& t2) : first(t1), second(t2)
    {
    }
    template < typename U1, typename U2 >
    inline tuple(const tuple< U1, U2 >& t) : first(t.first)
                                           , second(t.second)
    {
    }
};

template < typename T1, typename T2, typename T3 >
struct tuple< T1, T2, T3, void, void >
{
    T1 first;
    T2 second;
    T3 third;
    inline tuple() : first(), second(), third()
    {
    }
    inline tuple(const T1& t1, const T2& t2, const T3& t3) : first(t1), second(t2), third(t3)
    {
    }
    template < typename U1, typename U2, typename U3 >
    inline tuple(const tuple< U1, U2, U3 >& t) : first(t.first)
                                               , second(t.second)
                                               , third(t.third)
    {
    }
};

template < typename T1, typename T2, typename T3, typename T4 >
struct tuple< T1, T2, T3, T4, void >
{
    T1 first;
    T2 second;
    T3 third;
    T4 fourth;
    inline tuple() : first(), second(), third(), fourth()
    {
    }
    inline tuple(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
        : first(t1)
        , second(t2)
        , third(t3)
        , fourth(t4)
    {
    }
    template < typename U1, typename U2, typename U3, typename U4 >
    inline tuple(const tuple< U1, U2, U3, U4 >& t)
        : first(t.first)
        , second(t.second)
        , third(t.third)
        , fourth(t.fourth)
    {
    }
};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
struct tuple
{
    T1 first;
    T2 second;
    T3 third;
    T4 fourth;
    T5 fifth;
    inline tuple() : first(), second(), third(), fourth(), fifth()
    {
    }
    inline tuple(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
        : first(t1)
        , second(t2)
        , third(t3)
        , fourth(t4)
        , fifth(t5)
    {
    }
    template < typename U1, typename U2, typename U3, typename U4, typename U5 >
    inline tuple(const tuple< U1, U2, U3, U4, U5 >& t)
        : first(t.first)
        , second(t.second)
        , third(t.third)
        , fourth(t.fourth)
        , fifth(t.fifth)
    {
    }
};

template < typename T1, typename T2 >
static inline tuple< T1, T2 > make_tuple(const T1& t1, const T2& t2);

template < typename T1, typename T2, typename T3 >
static inline tuple< T1, T2, T3 > make_tuple(const T1& t1, const T2& t2, const T3& t3);

template < typename T1, typename T2, typename T3, typename T4 >
static inline tuple< T1, T2, T3, T4 > make_tuple(const T1& t1, const T2& t2, const T3& t3,
                                                 const T4& t4);

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
static inline tuple< T1, T2, T3, T4, T5 > make_tuple(const T1& t1, const T2& t2, const T3& t3,
                                                     const T4& t4, const T5& t5);

}  // namespace minitl

#include <bugengine/minitl/inl/tuple.inl>

/**************************************************************************************************/
#endif
