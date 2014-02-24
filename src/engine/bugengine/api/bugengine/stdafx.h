/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_BUGENGINE_STDAFX_H_
#define BE_BUGENGINE_STDAFX_H_
/**************************************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <filesystem/stdafx.h>
#include    <resource/stdafx.h>
#include    <scheduler/stdafx.h>
#include    <plugin/stdafx.h>
#include    <world/stdafx.h>

#if defined(building_bugengine) || defined(BUGENGINE_EXPORTS)
# define     BUGENGINEEXPORT          BE_EXPORT
#else
# define     BUGENGINEEXPORT          BE_IMPORT
#endif

/**************************************************************************************************/
#endif
