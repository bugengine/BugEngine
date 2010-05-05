/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_STDAFX_H_
#define BE_SYSTEM_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>

#if defined(building_system) || defined(SYSTEM_EXPORTS)
# define    SYSTEMEXPORT        BE_EXPORT
#elif defined(system_dll)
# define    SYSTEMEXPORT        BE_IMPORT
#else
# define    SYSTEMEXPORT
#endif

/*****************************************************************************/
#endif
