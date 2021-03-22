/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_STDAFX_H_
#define BE_BUGENGINE_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/meta/stdafx.h>
#include <bugengine/plugin/stdafx.h>
#include <bugengine/resource/stdafx.h>
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/world/stdafx.h>

#if defined(building_bugengine)
#    define BE_API_BUGENGINE BE_EXPORT
#elif defined(be_dll_bugengine)
#    define BE_API_BUGENGINE BE_IMPORT
#else
#    define BE_API_BUGENGINE
#endif

/**************************************************************************************************/
#endif
