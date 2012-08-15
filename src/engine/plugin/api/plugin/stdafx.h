/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_STDAFX_H_
#define BE_PLUGIN_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <filesystem/stdafx.h>
#include    <resource/stdafx.h>
#include    <scheduler/stdafx.h>

#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/value.inl>

#if defined(building_plugin) || defined(PLUGIN_EXPORTS)
# define    PLUGINEXPORT        BE_EXPORT
#elif defined(system_dll)
# define    PLUGINEXPORT        BE_IMPORT
#else
# define    PLUGINEXPORT
#endif

/*****************************************************************************/
#endif
