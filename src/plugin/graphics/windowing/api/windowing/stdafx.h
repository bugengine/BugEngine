/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WINDOWING_STDAFX_H_
#define BE_WINDOWING_STDAFX_H_
/**************************************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <3d/stdafx.h>

#if defined(building_windowing)
# define     BE_API_WINDOWING       BE_EXPORT
#elif defined(be_dll_windowing)
# define     BE_API_WINDOWING       BE_IMPORT
#else
# define     BE_API_WINDOWING
#endif

/**************************************************************************************************/
#endif
