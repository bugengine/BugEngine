/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_HH_
#define BE_CORE_PLATFORMS_HH_
/*****************************************************************************/

#ifndef _WIN32
# define OutputDebugString(s) printf("%s", s)
#endif


#ifdef _PPC

/* POWERPC*********************/
# define    BE_BIGENDIAN
# ifdef _PPC64
#  define   BE_64
# else
#  define   BE_32
# endif

#elif defined(_ARM)

/* ARM Big and little endian **/
# if defined(_ARMEL)
#  define   BE_LITTLEENDIAN
# else
#  define   BE_BIGENDIAN
# endif

#elif defined(_X86)

/* x86 ************************/
# define    BE_32
# define    BE_LITTLEENDIAN

#elif defined(_AMD64)

/* amd64 **********************/
# define    BE_32
# define    BE_LITTLEENDIAN

#elif defined(_MIPS)

/* MIPS Big and little endian */
# if defined(_MIPSEL)
#  define   BE_LITTLEENDIAN
# else
#  define   BE_BIGENDIAN
# endif
# if defined(_MIPS64)
#  define   BE_64
# else
#  define   BE_32
# endif

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
