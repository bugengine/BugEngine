/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_DEBUG_RUNTIME_STDAFX_H_
#define BE_DEBUG_RUNTIME_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/stdafx.h>

#if defined(building_runtime)
#    define BE_API_RUNTIME BE_EXPORT
#elif defined(be_dll_runtime)
#    define BE_API_RUNTIME BE_IMPORT
#else
#    define BE_API_RUNTIME
#endif

/**************************************************************************************************/
#endif
