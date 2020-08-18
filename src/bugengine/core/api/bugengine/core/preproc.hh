/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_PREPROCESSOR_H_
#define BE_CORE_PREPROCESSOR_H_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>

#define BE_STRINGIZE_IMPL(n) #n
#define BE_STRINGIZE(n)      BE_STRINGIZE_IMPL(n)
#define BE_CONCAT_IMPL(i, j) i##j
#define BE_CONCAT(i, j)      BE_CONCAT_IMPL(i, j)

/**************************************************************************************************/
#endif
