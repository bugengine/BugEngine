/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_STDAFX_H_
#define BE_GRAPHICS_STDAFX_H_
/*****************************************************************************/

#include <core/stdafx.h>
#include <rtti/stdafx.h>
#include <system/stdafx.h>
#include <input/stdafx.h>

#include <rtti/helper.hh>

#if defined(building_graphics) || defined(GRAPHICS_EXPORTS)
# define    GRAPHICSEXPORT      BE_EXPORT
#elif defined(graphics_dll)
# define    GRAPHICSEXPORT      BE_IMPORT
#else
# define    GRAPHICSEXPORT
#endif


/*****************************************************************************/
#endif
