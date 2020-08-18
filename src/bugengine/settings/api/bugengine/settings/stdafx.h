/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SETTINGS_STDAFX_H_
#define BE_SETTINGS_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rttiparse/stdafx.h>

#if defined(building_settings)
#    define BE_API_SETTINGS BE_EXPORT
#elif defined(be_dll_settings)
#    define BE_API_SETTINGS BE_IMPORT
#else
#    define BE_API_SETTINGS
#endif

/**************************************************************************************************/
#endif
