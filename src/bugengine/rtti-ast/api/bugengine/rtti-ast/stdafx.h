/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_STDAFX_H_
#define BE_RTTI_AST_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/rtti/stdafx.h>

#if defined(building_rtti_ast)
#    define BE_API_RTTI_AST BE_EXPORT
#elif defined(be_dll_rtti_ast)
#    define BE_API_RTTI_AST BE_IMPORT
#else
#    define BE_API_RTTI_AST
#endif

/**************************************************************************************************/
#endif
