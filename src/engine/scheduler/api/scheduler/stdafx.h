/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
#elif defined(system_dll)
# define    SCHEDULEREXPORT     BE_IMPORT
#else
# define    SCHEDULEREXPORT
#endif

namespace BugEngine { namespace Arena
{
be_api(SCHEDULER) minitl::Allocator& task();
}}

/*****************************************************************************/
#endif
