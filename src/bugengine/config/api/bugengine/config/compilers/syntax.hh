/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_COMPILERS_SYNTAX_HH_
#define BE_CONFIG_COMPILERS_SYNTAX_HH_
/**************************************************************************************************/

#define be_alignof(t) __alignof__(t)
#define be_break()

typedef signed char       i8;
typedef signed short      i16;
typedef signed int        i32;
typedef signed long int   i64;
typedef unsigned char     u8;
typedef unsigned short    u16;
typedef unsigned int      u32;
typedef unsigned long int u64;
typedef u8                byte;

typedef unsigned long int size_t;
typedef unsigned long int ptrdiff_t;
typedef unsigned long int intptr_t;

#ifndef __cplusplus
#    define be_restrict restrict
#else
#    define be_restrict __restrict
#endif

#define override
#define BE_NOINLINE
#define BE_ALWAYSINLINE        inline
#define BE_SUPPORTS_EXCEPTIONS 1

#define BE_EXPORT
#define BE_IMPORT

/**************************************************************************************************/
#endif
