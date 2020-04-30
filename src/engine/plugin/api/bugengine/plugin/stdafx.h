/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PLUGIN_STDAFX_H_
#define BE_PLUGIN_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/resource/stdafx.h>
#include <bugengine/rtti/stdafx.h>
#include <bugengine/scheduler/stdafx.h>

#if defined(building_plugin)
#    define BE_API_PLUGIN BE_EXPORT
#elif defined(be_dll_plugin)
#    define BE_API_PLUGIN BE_IMPORT
#else
#    define BE_API_PLUGIN
#endif

/**************************************************************************************************/
#endif
