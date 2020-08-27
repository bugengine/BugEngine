/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CONFIG_PLATFORM_IPHONE_HH_
#define BE_CONFIG_PLATFORM_IPHONE_HH_
/**************************************************************************************************/

#define BE_PLATFORM_NAME   iPhone
#define BE_PLATFORM_IPHONE 1
#define BE_PLATFORM_MACOS  1

#include <malloc/malloc.h>
#define malloca    alloca
#define freea(p)   (void)p
#define DIRENT_H   <dirent.h>
#define PLUGIN_EXT ".dylib"

/**************************************************************************************************/
#endif
