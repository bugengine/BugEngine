/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_PLATFORM_DARWIN_HH_
#define BE_CORE_PLATFORMS_PLATFORM_DARWIN_HH_
/**************************************************************************************************/

#define BE_PLATFORM_NAME  MacOSX
#define BE_PLATFORM_MACOS 1
#define BE_PLATFORM_PC    1

#include <malloc/malloc.h>
#define malloca  alloca
#define freea(p) (void)p

#define DIRENT_H   <sys/dir.h>
#define PLUGIN_EXT ".dylib"

#include <AvailabilityMacros.h>
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1060
#    if !defined(AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER)
#        define AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER UNAVAILABLE_ATTRIBUTE
#    endif
#endif
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1070
#    if !defined(AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER)
#        define AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER UNAVAILABLE_ATTRIBUTE
#    endif
#endif
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080
#    if !defined(AVAILABLE_MAC_OS_X_VERSION_10_8_AND_LATER)
#        define AVAILABLE_MAC_OS_X_VERSION_10_8_AND_LATER UNAVAILABLE_ATTRIBUTE
#    endif
#endif
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1090
#    if !defined(AVAILABLE_MAC_OS_X_VERSION_10_9_AND_LATER)
#        define AVAILABLE_MAC_OS_X_VERSION_10_9_AND_LATER UNAVAILABLE_ATTRIBUTE
#    endif
#endif

/**************************************************************************************************/
#endif
