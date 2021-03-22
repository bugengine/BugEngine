/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_PARSE_STDAFX_H_
#define BE_META_PARSE_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/introspect/stdafx.h>
#include <bugengine/meta/stdafx.h>

#if defined(building_reflection)
#    define BE_API_REFLECTION BE_EXPORT
#elif defined(be_dll_reflection)
#    define BE_API_REFLECTION BE_IMPORT
#else
#    define BE_API_REFLECTION
#endif

/**************************************************************************************************/
#endif
