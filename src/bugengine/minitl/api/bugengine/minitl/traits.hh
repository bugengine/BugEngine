/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_TRAITS_HH_
#define BE_MINITL_TRAITS_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <string.h>

namespace minitl {

template < typename T >
struct less;

template <>
struct less< const char* >
{
    bool operator()(const char* str1, const char* str2) const
    {
        return ::strcmp(str1, str2) < 0;
    }
};

}  // namespace minitl

/**************************************************************************************************/
#endif
