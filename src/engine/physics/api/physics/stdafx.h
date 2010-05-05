/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICS_STDAFX_H_
#define BE_PHYSICS_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>


#if defined(building_physics) || defined(PHYSICS_EXPORTS)
# define    PHYSICSEXPORT       BE_EXPORT
#elif defined(physics_dll)
# define    PHYSICSEXPORT       BE_IMPORT
#else
# define    PHYSICSEXPORT
#endif

/*****************************************************************************/
#endif
