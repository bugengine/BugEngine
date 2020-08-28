/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CONFIG_COMPILERS_SUNCC_HH_
#define BE_CONFIG_COMPILERS_SUNCC_HH_
/**************************************************************************************************/

#define be_alignof(t) __alignof__(t)
#if defined(_X86) || defined(_AMD64)
#    define be_break() asm("int $3")
#else
#    error Platform not supported
#endif

#include <alloca.h>
#include <stdint.h>
#include <stdlib.h>
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u8       byte;

#ifndef __cplusplus
#    define be_restrict restrict
#else
#    define be_restrict __restrict
#endif

#ifndef alloca
#    define alloca(x) __builtin_alloca(x)
extern "C" void* __builtin_alloca(size_t);
#endif
#ifdef __EXCEPTIONS
#    define BE_SUPPORTS_EXCEPTIONS 1
#else
#    define BE_SUPPORTS_EXCEPTIONS 0
#endif

#undef __REDIRECT
#ifdef __Cplusplus
#    include <cerrno>
#else
#    include <errno.h>
#endif

#define override
#define BE_NOINLINE
#define BE_ALWAYSINLINE inline

#define BE_EXPORT __global
#define BE_IMPORT

/**************************************************************************************************/
#endif
