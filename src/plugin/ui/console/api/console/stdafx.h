/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_STDAFX_H_
#define BE_UI_CONSOLE_STDAFX_H_
/**************************************************************************************************/
#include    <bugengine/stdafx.h>


#if defined(building_console)
# define    CONSOLEEXPORT  BE_EXPORT
#else
# define    CONSOLEEXPORT  BE_IMPORT
#endif

/**************************************************************************************************/
#endif
