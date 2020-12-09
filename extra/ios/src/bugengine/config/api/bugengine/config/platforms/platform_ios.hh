/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CONFIG_PLATFORM_IOS_HH_
#define BE_CONFIG_PLATFORM_IOS_HH_
/**************************************************************************************************/

#define BE_PLATFORM_NAME  iOS
#define BE_PLATFORM_IOS   1
#define BE_PLATFORM_MACOS 1

#include <malloc/malloc.h>
#define malloca    alloca
#define freea(p)   (void)p
#define DIRENT_H   <dirent.h>
#define PLUGIN_EXT ".dylib"

/**************************************************************************************************/
#endif
