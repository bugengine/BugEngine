/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_INPUT_STDAFX_H_
#define BE_INPUT_STDAFX_H_
/*****************************************************************************/

#include    <bugengine/stdafx.h>


#if defined(building_input) || defined(INPUT_EXPORTS)
# define    INPUTEXPORT         BE_EXPORT
#else
# define    INPUTEXPORT         BE_IMPORT
#endif

/*****************************************************************************/
#endif
