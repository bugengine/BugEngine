/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STDAFX_H_
#define BE_WORLD_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/resource/stdafx.h>
#include <bugengine/rtti/stdafx.h>
#include <bugengine/scheduler/stdafx.h>

#if defined(building_world)
#    define BE_API_WORLD BE_EXPORT
#elif defined(be_dll_world)
#    define BE_API_WORLD BE_IMPORT
#else
#    define BE_API_WORLD
#endif

#ifndef BE_COMPUTE
namespace BugEngine { namespace Arena {
be_api(WORLD) minitl::Allocator& game();
}}  // namespace BugEngine::Arena
#endif

/**************************************************************************************************/
#endif
