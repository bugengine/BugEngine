/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_COMPILERS_SUNCC_H_
#define BE_CORE_COMPILERS_SUNCC_H_
/*****************************************************************************/

#define be_alignof(t)          __alignof__(t)
#if defined(_X86)||defined(_AMD64)
# define be_break()              __asm("int3")
#else
# error Platform not supported
#endif

# include <stdint.h>
# include <stdlib.h>
typedef int8_t                 i8;
typedef int16_t                i16;
typedef int32_t                i32;
typedef int64_t                i64;
typedef uint8_t                u8;
typedef uint16_t               u16;
typedef uint32_t               u32;
typedef uint64_t               u64;
typedef u8                      byte;

#define    override
#define BE_THREAD_LOCAL        __thread
#define BE_NOINLINE            
#define BE_ALWAYSINLINE        inline

#define BE_EXPORT
#define BE_IMPORT

#undef __REDIRECT

/*****************************************************************************/
#endif
