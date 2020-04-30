/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_STDAFX_H_
#define BE_SCHEDULER_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/resource/stdafx.h>
#include <bugengine/rtti/stdafx.h>

#if defined(building_scheduler)
#    define BE_API_SCHEDULER BE_EXPORT
#elif defined(be_dll_scheduler)
#    define BE_API_SCHEDULER BE_IMPORT
#else
#    define BE_API_SCHEDULER
#endif

#ifndef BE_BUILD_KERNEL
namespace BugEngine { namespace Arena {
be_api(SCHEDULER) minitl::Allocator& task();
}}  // namespace BugEngine::Arena
#endif

#define be_product(x)

/**************************************************************************************************/
#endif
