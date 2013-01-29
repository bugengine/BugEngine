/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_STDAFX_H_
#define BE_FILESYSTEM_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>

#if defined(building_filesystem) || defined(FILESYSTEM_EXPORTS)
# define    FILESYSTEMEXPORT    BE_EXPORT
#else
# define    FILESYSTEMEXPORT    BE_IMPORT
#endif

namespace BugEngine { namespace Arena
{
be_api(FILESYSTEM) minitl::Allocator& filesystem();
}}

/*****************************************************************************/
#endif
