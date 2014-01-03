/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PLUGIN_STDAFX_H_
#define BE_PLUGIN_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <filesystem/stdafx.h>
#include    <resource/stdafx.h>
#include    <scheduler/stdafx.h>

#if defined(building_plugin) || defined(PLUGIN_EXPORTS)
# define    PLUGINEXPORT        BE_EXPORT
#else
# define    PLUGINEXPORT        BE_IMPORT
#endif

/*****************************************************************************/
#endif
