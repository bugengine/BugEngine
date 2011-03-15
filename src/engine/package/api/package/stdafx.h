/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_STDAFX_H_
#define BE_PACKAGE_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>


#if defined(building_package) || defined(PACKAGE_EXPORTS)
# define     PKGEXPORT           BE_EXPORT
#elif defined(rtti_dll)
# define     PKGEXPORT           BE_IMPORT
#else
# define     PKGEXPORT
#endif


/*****************************************************************************/
#endif
