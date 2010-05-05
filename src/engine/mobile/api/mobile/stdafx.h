/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_STDAFX_H_
#define BE_MOBILE_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <system/stdafx.h>
#include    <rtti/stdafx.h>
#include    <graphics/stdafx.h>
#include    <physics/stdafx.h>
#include    <audio/stdafx.h>


#if defined(building_mobile) || defined(MOBILE_EXPORTS)
# define    MOBILEEXPORT        BE_EXPORT
#elif defined(mobile_dll)
# define    MOBILEEXPORT        BE_IMPORT
#else
# define    MOBILEEXPORT
#endif

/*****************************************************************************/
#endif
