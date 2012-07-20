/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_DEBUG_STDAFX_H_
#define BE_DEBUG_STDAFX_H_
/*****************************************************************************/

#include    <kernel/stdafx.h>

#define     be_api(module) module##EXPORT

#if defined(building_debug) || defined(DEBUG_EXPORTS)
# define    DEBUGEXPORT         BE_EXPORT
#elif defined(core_dll)
# define    DEBUGEXPORT         BE_IMPORT
#else
# define    DEBUGEXPORT
#endif

/*****************************************************************************/
#endif
