/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STDAFX_H_
#define BE_WORLD_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <system/stdafx.h>
#include    <rtti/stdafx.h>

#if defined(building_world) || defined(WORLD_EXPORTS)
# define    WORLDEXPORT         BE_EXPORT
#elif defined(world_dll)
# define    WORLDEXPORT         BE_IMPORT
#else
# define    WORLDEXPORT
#endif

namespace BugEngine { namespace Arena
{
be_api(WORLD) minitl::Allocator& game();
}}

/*****************************************************************************/
#endif
