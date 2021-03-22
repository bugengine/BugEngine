/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_STDAFX_H_
#define BE_RESOURCE_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/meta/stdafx.h>

#if defined(building_resource)
#    define BE_API_RESOURCE BE_EXPORT
#elif defined(be_dll_resource)
#    define BE_API_RESOURCE BE_IMPORT
#else
#    define BE_API_RESOURCE
#endif

#ifndef BE_COMPUTE
namespace BugEngine { namespace Arena {
be_api(RESOURCE) minitl::Allocator& resource();
}}  // namespace BugEngine::Arena
#endif

/**************************************************************************************************/
#endif
