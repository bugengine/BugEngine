/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RESOURCE_STDAFX_H_
#define BE_RESOURCE_STDAFX_H_
/**************************************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <filesystem/stdafx.h>

#if defined(building_resource) || defined(RESOURCE_EXPORTS)
# define    RESOURCEEXPORT      BE_EXPORT
#else
# define    RESOURCEEXPORT      BE_IMPORT
#endif

namespace BugEngine { namespace Arena
{
be_api(RESOURCE) minitl::Allocator& resource();
}}

/**************************************************************************************************/
#endif
