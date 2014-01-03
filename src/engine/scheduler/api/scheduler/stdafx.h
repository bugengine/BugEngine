/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_STDAFX_H_
#define BE_SCHEDULER_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <filesystem/stdafx.h>
#include    <resource/stdafx.h>

#if defined(building_scheduler) || defined(SCHEDULER_EXPORTS)
# define    SCHEDULEREXPORT     BE_EXPORT
#else
# define    SCHEDULEREXPORT     BE_IMPORT
#endif

namespace BugEngine { namespace Arena
{
be_api(SCHEDULER) minitl::Allocator& task();
}}

#define be_product(x)

/*****************************************************************************/
#endif
