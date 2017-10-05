/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SETTINGS_STDAFX_H_
#define BE_SETTINGS_STDAFX_H_
/**************************************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>

#if defined(building_settings) || defined(SETTINGS_EXPORTS)
# define     SETTINGSEXPORT      BE_EXPORT
#else
# define     SETTINGSEXPORT      BE_IMPORT
#endif

/**************************************************************************************************/
#endif
