/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_STDAFX_H_
#define BE_INTROSPECT_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/meta/stdafx.h>

#if defined(building_introspect)
#    define BE_API_INTROSPECT BE_EXPORT
#elif defined(be_dll_introspect)
#    define BE_API_INTROSPECT BE_IMPORT
#else
#    define BE_API_INTROSPECT
#endif

/**************************************************************************************************/
#endif
