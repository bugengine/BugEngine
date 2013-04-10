/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
