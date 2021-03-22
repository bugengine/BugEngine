/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SETTINGS_STDAFX_H_
#define BE_SETTINGS_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/meta/stdafx.h>
#include <bugengine/reflection/stdafx.h>

#if defined(building_settings)
#    define BE_API_SETTINGS BE_EXPORT
#elif defined(be_dll_settings)
#    define BE_API_SETTINGS BE_IMPORT
#else
#    define BE_API_SETTINGS
#endif

/**************************************************************************************************/
#endif
