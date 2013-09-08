/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_COMPILERS_SYNTAX_HH_
#define BE_KERNEL_COMPILERS_SYNTAX_HH_
/*****************************************************************************/

#define be_alignof(t)           __alignof__(t)
#define be_break()

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long int     i64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long int   u64;
typedef u8                  byte;

typedef unsigned long int	size_t;
typedef unsigned long int   ptrdiff_t;
typedef unsigned long int   intptr_t;

#define override
#define BE_NOINLINE
#define BE_ALWAYSINLINE
#define BE_SELECTOVERLOAD(o)

#define BE_EXPORT
#define BE_IMPORT

/*****************************************************************************/
#endif
