/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WIN32_STDAFX_H_
#define BE_WIN32_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <graphics/stdafx.h>

#include    <win32/resource.h>

#if defined(_WIN32)
# if defined(building_w32) || defined(W32_EXPORTS)
#  define   W32EXPORT         __declspec(dllexport)
# elif defined(w32_dll)
#  define   W32EXPORT         __declspec(dllimport)
# else
#  define   W32EXPORT
# endif
#else
# define    W32EXPORT
#endif

/*****************************************************************************/
#endif
