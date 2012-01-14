/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_STDAFX_H_
#define BE_3D_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>
#include    <world/stdafx.h>


#if defined(building_3d)
# define    _3DEXPORT      BE_EXPORT
#elif defined(_3d_dll)
# define    _3DEXPORT      BE_IMPORT
#else
# define    _3DEXPORT
#endif

/*****************************************************************************/
#endif
