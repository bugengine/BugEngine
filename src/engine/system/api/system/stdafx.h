/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_STDAFX_H_
#define BE_SYSTEM_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>

#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/value.inl>

#if defined(building_system) || defined(SYSTEM_EXPORTS)
# define    SYSTEMEXPORT        BE_EXPORT
#elif defined(system_dll)
# define    SYSTEMEXPORT        BE_IMPORT
#else
# define    SYSTEMEXPORT
#endif

namespace BugEngine { namespace Arena
{
be_api(SYSTEM) Allocator& filesystem();
be_api(SYSTEM) Allocator& task();
be_api(SYSTEM) Allocator& resource();
}}

/*****************************************************************************/
#endif
