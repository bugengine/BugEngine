/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GUI_GTK3_STDAFX_H_
#define BE_GUI_GTK3_STDAFX_H_
/**************************************************************************************************/
#include <bugengine/stdafx.h>

#if defined(building_gtk3)
#    define BE_API_GTK3 BE_EXPORT
#elif defined(be_dll_gtk3)
#    define BE_API_GTK3 BE_IMPORT
#else
#    define BE_API_GTK3
#endif

/**************************************************************************************************/
#endif
