/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_STDAFX_H_
#define BE_PACKAGE_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>
#include    <bugengine/stdafx.h>

#if defined(building_package)
# define    PACKAGEEXPORT      BE_EXPORT
#elif defined(package_dll)
# define    PACKAGEEXPORT      BE_IMPORT
#else
# define    PACKAGEEXPORT
#endif

namespace BugEngine
{

Allocator& packageArena();

namespace PackageBuilder
{
Allocator& packageBuilderArena();
}

}

/*****************************************************************************/
#endif
