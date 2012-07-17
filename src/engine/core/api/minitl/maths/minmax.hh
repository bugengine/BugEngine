/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_MATHS_MINMAX_
#define BE_MINITL_MATHS_MINMAX_
/*****************************************************************************/

namespace minitl
{

template< size_t v1, size_t v2, bool v1mtv2 = (v1>v2) >
struct compare;

template< size_t v1, size_t v2 >
struct compare< v1, v2, true >
{
    enum
    {
        Max = v1,
        Min = v2
    };
};

template< size_t v1, size_t v2 >
struct compare< v1, v2, false >
{
    enum
    {
        Max = v2,
        Min = v1
    };
};

template< size_t v1, size_t v2 >
struct max
{
    enum { Value = compare<v1, v2>::Max };
};

template< size_t v1, size_t v2 >
struct min
{
    enum { Value = compare<v1, v2>::Min };
};

}


/*****************************************************************************/
#endif
