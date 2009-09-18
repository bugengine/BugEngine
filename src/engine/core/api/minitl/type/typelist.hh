/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_TYPE_TYPELIST_
#define BE_MINITL_TYPE_TYPELIST_
/*****************************************************************************/
#include    <minitl/maths/minmax.hh>

namespace minitl
{

template< typename _Type, typename _Tail>
struct typelist
{
    typedef _Type   Type;
    typedef _Tail   Tail;
    enum
    {
        IndexOf = 1+Tail::IndexOf,
        SizeOf  = max< sizeof(_Type), Tail::SizeOf >::Value
    };
};

template< typename _Type >
struct typelist< _Type, void >
{
    typedef _Type       Type;
    typedef void        Tail;
    enum
    {
        IndexOf = 1,
        SizeOf  = sizeof(_Type)
    };
};


template< typename T, typename TypeList >
struct indexof;
template< typename T, typename Tail >
struct indexof< T, typelist< T, Tail > >
{
    enum
    {
        Value = typelist< T, Tail >::IndexOf
    };
};
template< typename T, typename T2, typename Tail >
struct indexof< T, typelist< T2, Tail > >
{
    enum
    {
        Value = indexof< T, Tail >::Value
    };
};
template< >
struct indexof< void, void >
{
    enum
    {
        Value = 0
    };
};


#define TYPELIST1(t)                                        minitl::typelist< t, void >
#define TYPELIST2(t1,t2)                                    minitl::typelist< t1, TYPELIST1(t2) >
#define TYPELIST3(t1,t2,t3)                                 minitl::typelist< t1, TYPELIST2(t2,t3) >
#define TYPELIST4(t1,t2,t3,t4)                              minitl::typelist< t1, TYPELIST3(t2,t3,t4) >
#define TYPELIST5(t1,t2,t3,t4,t5)                           minitl::typelist< t1, TYPELIST4(t2,t3,t4,t5) >
#define TYPELIST6(t1,t2,t3,t4,t5,t6)                        minitl::typelist< t1, TYPELIST5(t2,t3,t4,t5,t6) >
#define TYPELIST7(t1,t2,t3,t4,t5,t6,t7)                     minitl::typelist< t1, TYPELIST6(t2,t3,t4,t5,t6,t7) >
#define TYPELIST8(t1,t2,t3,t4,t5,t6,t7,t8)                  minitl::typelist< t1, TYPELIST7(t2,t3,t4,t5,t6,t7,t8) >
#define TYPELIST9(t1,t2,t3,t4,t5,t6,t7,t8,t9)               minitl::typelist< t1, TYPELIST8(t2,t3,t4,t5,t6,t7,t8,t9) >
#define TYPELIST10(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10)          minitl::typelist< t1, TYPELIST9(t2,t3,t4,t5,t6,t7,t8,t9,t10) >


}

/*****************************************************************************/
#endif
