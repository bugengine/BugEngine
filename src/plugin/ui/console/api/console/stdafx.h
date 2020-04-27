/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_STDAFX_H_
#define BE_UI_CONSOLE_STDAFX_H_
/**************************************************************************************************/
#include    <bugengine/stdafx.h>


#if defined(building_console)
# define    BE_API_CONSOLE  BE_EXPORT
#elif defined(be_dll_console)
# define    BE_API_CONSOLE  BE_IMPORT
#else
# define    BE_API_CONSOLE
#endif

/**************************************************************************************************/
#endif
