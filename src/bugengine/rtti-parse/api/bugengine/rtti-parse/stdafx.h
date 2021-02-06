/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PARSE_STDAFX_H_
#define BE_RTTI_PARSE_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti/stdafx.h>

#if defined(building_rtti_parse)
#    define BE_API_RTTI_PARSE BE_EXPORT
#elif defined(be_dll_rtti_parse)
#    define BE_API_RTTI_PARSE BE_IMPORT
#else
#    define BE_API_RTTI_PARSE
#endif

/**************************************************************************************************/
#endif
