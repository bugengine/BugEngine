/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_DEBUG_RUNTIME_STDAFX_H_
#define BE_DEBUG_RUNTIME_STDAFX_H_
/*****************************************************************************/

#include    <debug/stdafx.h>
#include    <core/stdafx.h>

#if defined(building_runtime) || defined(RUNTIME_EXPORTS)
# define     RUNTIMEEXPORT          BE_EXPORT
#elif defined(bugengine_dll)
# define     RUNTIMEEXPORT          BE_IMPORT
#else
# define     RUNTIMEEXPORT
#endif

/*****************************************************************************/
#endif

