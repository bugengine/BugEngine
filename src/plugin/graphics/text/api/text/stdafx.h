/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_STDAFX_H_
#define BE_TEXT_STDAFX_H_
/*****************************************************************************/

#include    <bugengine/stdafx.h>
#include    <3d/stdafx.h>

#if defined(building_text)
# define    TEXTEXPORT          BE_EXPORT
#elif defined(text_dll)
# define    TEXTEXPORT          BE_IMPORT
#else
# define    TEXTEXPORT
#endif

/*****************************************************************************/
#endif
