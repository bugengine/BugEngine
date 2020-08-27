/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CONFIG_PLATFORMS_HH_
#define BE_CONFIG_PLATFORMS_HH_
/**************************************************************************************************/

#ifndef _WIN32
#    define OutputDebugString(s) printf("%s", s)
#endif

#ifdef _PPC

/* POWERPC ********************/
#    define BE_BIGENDIAN
#    ifdef _PPC64
#        define BE_64
#    else
#        define BE_32
#    endif

#elif defined(_ARM)

/* ARM Big and little endian **/
#    if defined(_ARMEL)
#        define BE_LITTLEENDIAN
#    else
#        define BE_BIGENDIAN
#    endif

#elif defined(_X86)

/* x86 ************************/
#    define BE_32
#    define BE_LITTLEENDIAN

#elif defined(_AMD64)

/* amd64 **********************/
#    define BE_64
#    define BE_LITTLEENDIAN

#elif defined(_MIPS)

/* MIPS Big and little endian */
#    if defined(_MIPSEL)
#        define BE_LITTLEENDIAN
#    else
#        define BE_BIGENDIAN
#    endif
#    if defined(_MIPS64)
#        define BE_64
#    else
#        define BE_32
#    endif

/* ARM 64bits *****************/
#elif defined(_ARM64)
#    define BE_64
#    define BE_LITTLEENDIAN

#else
#    error "unknown arch"
#endif

#ifndef BE_PLATFORM
#    error "Unknown platform: you need to define BE_PLATFORM"
#else
// clang-format off
#    define BE_PLATFORM_INCLUDE_ bugengine/config/platforms/BE_PLATFORM.hh
// clang-format on
#    define BE_STRINGIZE__(x)  #x
#    define BE_STRINGIZE_(x)   BE_STRINGIZE__(x)
#    include BE_STRINGIZE_(BE_PLATFORM_INCLUDE_)
#    undef BE_PLATFORM_INCLUDE_
#    undef BE_STRINGIZE_
#    undef BE_STRINGIZE__
#endif

/**************************************************************************************************/
#endif
