/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_STDAFX_H_
#define BE_FILESYSTEM_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/rtti/stdafx.h>

#if defined(building_filesystem)
#    define BE_API_FILESYSTEM BE_EXPORT
#elif defined(be_dll_filesystem)
#    define BE_API_FILESYSTEM BE_IMPORT
#else
#    define BE_API_FILESYSTEM
#endif

#ifndef BE_COMPUTE
namespace BugEngine { namespace Arena {
be_api(FILESYSTEM) minitl::Allocator& filesystem();
}}  // namespace BugEngine::Arena
#endif

/**************************************************************************************************/
#endif
