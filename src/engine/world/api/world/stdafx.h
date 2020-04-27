/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_STDAFX_H_
#define BE_WORLD_STDAFX_H_
/**************************************************************************************************/

#include    <core/stdafx.h>
#include    <filesystem/stdafx.h>
#include    <resource/stdafx.h>
#include    <scheduler/stdafx.h>
#include    <rtti/stdafx.h>

#if defined(building_world)
# define    BE_API_WORLD        BE_EXPORT
#elif defined(be_dll_world)
# define    BE_API_WORLD        BE_IMPORT
#else
# define    BE_API_WORLD
#endif

#ifndef BE_BUILD_KERNEL
namespace BugEngine { namespace Arena
{
be_api(WORLD) minitl::Allocator& game();
}}
#endif

/**************************************************************************************************/
#endif
