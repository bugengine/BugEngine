/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_STDAFX_H_
#define BE_PACKAGE_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>

#if defined(building_package)
# define    BE_API_PACKAGE  BE_EXPORT
#elif defined(be_dll_package)
# define    BE_API_PACKAGE  BE_IMPORT
#else
# define    BE_API_PACKAGE
#endif

#ifndef BE_BUILD_KERNEL
namespace BugEngine { namespace Arena
{

minitl::Allocator& package();
minitl::Allocator& packageBuilder();

}}
#endif

/**************************************************************************************************/
#endif
