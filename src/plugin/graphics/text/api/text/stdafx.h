/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_STDAFX_H_
#define BE_TEXT_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>
#include    <3d/stdafx.h>

#if defined(building_text)
# define    BE_API_TEXT         BE_EXPORT
#elif defined(be_dll_text)
# define    BE_API_TEXT         BE_IMPORT
#else
# define    BE_API_TEXT
#endif

/**************************************************************************************************/
#endif
