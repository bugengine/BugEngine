/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_STDAFX_H_
#define BE_PACKAGE_STDAFX_H_
/*****************************************************************************/

#include    <bugengine/stdafx.h>

#if defined(building_package)
# define    PACKAGEEXPORT      BE_EXPORT
#elif defined(package_dll)
# define    PACKAGEEXPORT      BE_IMPORT
#else
# define    PACKAGEEXPORT
#endif

namespace BugEngine { namespace Arena
{

minitl::Allocator& package();
minitl::Allocator& packageBuilder();

}}

/*****************************************************************************/
#endif
