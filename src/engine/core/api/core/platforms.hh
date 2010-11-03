/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_HH_
#define BE_CORE_PLATFORMS_HH_
/*****************************************************************************/

#ifndef _WIN32
# define OutputDebugString(s) printf("%s", s)
#endif

#if defined(_AMD64) || defined(_PPC64)
# define BE_64
#elif defined(_X86) || defined(_PPC)
# define BE_32
#elif defined(_ARM)
# define BE_32
#else
# error "unknown arch"
#endif

#ifndef BE_PLATFORM
# error "Unknown platform: you need to define BE_PLATFORM"
#else
# define BE_PLATFORM_INCLUDE_ core/platforms/BE_PLATFORM.hh
# include BE_STRINGIZE(BE_PLATFORM_INCLUDE_)
# undef BEPLATFORM_INCLUDE_
#endif

/*****************************************************************************/
#endif
