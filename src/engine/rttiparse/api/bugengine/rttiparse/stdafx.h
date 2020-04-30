/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_STDAFX_H_
#define BE_RTTIPARSE_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/rtti/stdafx.h>

#if defined(building_rttiparse)
#    define BE_API_RTTIPARSE BE_EXPORT
#elif defined(be_dll_rttiparse)
#    define BE_API_RTTIPARSE BE_IMPORT
#else
#    define BE_API_RTTIPARSE
#endif

/**************************************************************************************************/
#endif
