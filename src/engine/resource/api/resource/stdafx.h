/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_STDAFX_H_
#define BE_RESOURCE_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <filesystem/stdafx.h>

#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/value.inl>

#if defined(building_resource) || defined(RESOURCE_EXPORTS)
# define    RESOURCEEXPORT      BE_EXPORT
#elif defined(resource_dll)
# define    RESOURCEEXPORT      BE_IMPORT
#else
# define    RESOURCEEXPORT
#endif

namespace BugEngine { namespace Arena
{
be_api(RESOURCE) minitl::Allocator& resource();
}}

/*****************************************************************************/
#endif
