/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_DEBUG_RUNTIME_STDAFX_H_
#define BE_DEBUG_RUNTIME_STDAFX_H_
/*****************************************************************************/

#include    <bugengine/stdafx.h>


#if defined(building_runtime) || defined(RUNTIME_EXPORTS)
# define     RUNTIMEEXPORT          BE_EXPORT
#else
# define     RUNTIMEEXPORT          BE_IMPORT
#endif

/*****************************************************************************/
#endif

