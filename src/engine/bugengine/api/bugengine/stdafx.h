/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_STDAFX_H_
#define BE_BUGENGINE_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>
#include    <world/stdafx.h>

#if defined(building_bugengine) || defined(BUGENGINE_EXPORTS)
# define     BUGENGINEEXPORT          BE_EXPORT
#elif defined(bugengine_dll)
# define     BUGENGINEEXPORT          BE_IMPORT
#else
# define     BUGENGINEEXPORT
#endif

/*****************************************************************************/
#endif
