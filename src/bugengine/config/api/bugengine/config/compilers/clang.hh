/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_COMPILERS_CLANG_HH_
#define BE_CONFIG_COMPILERS_CLANG_HH_
/**************************************************************************************************/

#define be_alignof(t) __alignof__(t)
#if defined(_X86) || defined(_AMD64)
#    define be_break() __asm("int3")
#elif defined(_POWERPC)
#    define be_break() __asm("trap")
#elif defined(_MIPS)
#    define be_break() __asm("break")
#elif defined(_ARM64)
#    define be_break() __asm__ volatile("brk 0x0")
#elif defined(__APPLE_CC__) && defined(_ARM)
#    if !defined(__thumb__)
#        define be_break() __asm__ volatile(".word 0xe7f001f0");
#    else
#        define be_break() __asm__ volatile(".short 0xde01");
#    endif
#elif defined(_ARM) && !defined(__thumb__)
#    define be_break() __asm__ volatile(".inst 0xe7f001f0")
#elif defined(_ARM)
#    define be_break() __asm__ volatile(".inst 0xde01");
#else
#    error "Breakpoint not supported on this platform"
#    define be_break()
#endif

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

#if __GXX_EXPERIMENTAL_CXX0X__
#    define BE_HAS_MOVE_SEMANTICS
#endif

#ifndef _MSC_VER
#    define override
#endif
#define BE_NOINLINE     __attribute__((noinline))
#define BE_ALWAYSINLINE __attribute__((always_inline))
#ifdef __EXCEPTIONS
#    define BE_SUPPORTS_EXCEPTIONS 1
#else
#    define BE_SUPPORTS_EXCEPTIONS 0
#endif

#ifndef __cplusplus
#    define be_restrict restrict
#else
#    define be_restrict __restrict
#endif

#ifndef BE_STATIC
#    ifndef _WIN32
#        define BE_EXPORT __attribute__((visibility("default")))
#        define BE_IMPORT __attribute__((visibility("default")))
#    elif(__clang_major__ > 3) || (__clang_major__ == 3 && __clang_minor__ > 6)
#        define BE_EXPORT __declspec(dllexport)
#        define BE_IMPORT __declspec(dllimport)
#    else
#        define BE_EXPORT
#        define BE_IMPORT
#    endif
#else
#    define BE_EXPORT
#    define BE_IMPORT
#endif

/**************************************************************************************************/
#endif
