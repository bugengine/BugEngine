/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_COMPILERS_GCC_H_
#define BE_CORE_COMPILERS_GCC_H_
/*****************************************************************************/

#define be_alignof(t)           __alignof__(t)
#if defined(_X86)||defined(_AMD64)
# define be_break()              __asm("int3")
#elif defined(_POWERPC)
# define be_break()              __asm("trap")
#elif defined(_MIPS)
# define be_break()              __asm("break")
#else
# error "Breakpoint not supported on this platform"
#endif

#include <stdint.h>
#include <stdlib.h>
typedef int8_t                  i8;
typedef int16_t                 i16;
typedef int32_t                 i32;
typedef int64_t                 i64;
typedef uint8_t                 u8;
typedef uint16_t                u16;
typedef uint32_t                u32;
typedef uint64_t                u64;
typedef u8                      byte;

#define override
#define BE_THREAD_LOCAL         __thread
#define BE_NOINLINE             __attribute__((noinline))
#define BE_ALWAYSINLINE         __attribute__((always_inline))

#define BE_SET_ALIGNMENT(n)     __attribute__ ((aligned(n)))
#ifdef _WIN32
# define BE_EXPORT              __declspec(dllexport)
# define BE_IMPORT              __declspec(dllimport)
#else
# define BE_EXPORT              __attribute__ ((visibility("default")))
# define BE_IMPORT
#endif

/*****************************************************************************/
#endif
