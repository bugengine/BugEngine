/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_STDAFX_H_
#define BE_WINDOWING_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <graphics/stdafx.h>

#include    <windowing/win32/resource.h>

#if defined(building_windowing) || defined(WINDOWING_EXPORTS)
# define     WINDOWINGEXPORT          BE_EXPORT
#elif defined(rtti_dll)
# define     WINDOWINGEXPORT          BE_IMPORT
#else
# define     WINDOWINGEXPORT
#endif

/*****************************************************************************/
#endif
