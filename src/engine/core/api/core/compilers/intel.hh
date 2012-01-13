/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_COMPILERS_INTEL_H_
#define BE_CORE_COMPILERS_INTEL_H_
/*****************************************************************************/

#define be_alignof(t)           __alignof(t)
#define be_break()              __asm("int3")

typedef signed __int8           i8;
typedef signed __int16          i16;
typedef signed __int32          i32;
typedef signed __int64          i64;
typedef unsigned __int8         u8;
typedef unsigned __int16        u16;
typedef unsigned __int32        u32;
typedef unsigned __int64        u64;
typedef u8                      byte;


#pragma warning disable 161     // unknown pragma
#pragma warning disable 279     // controlling expression is constant
#pragma warning disable 383     // reference to copy
#pragma warning disable 424     // extra ;
#pragma warning disable 177     // variable was declared but never used
#pragma warning disable 593     // variable was set but never used
#pragma warning disable 981
#pragma warning disable 1418    // external function definition with no prior declaration
#pragma warning disable 2259
#pragma warning disable 411     // class X defines no constructor to initialize...
#pragma warning disable 2304    // explicit constructors
#pragma warning disable 2289    // signature for copy is Type(const Type&)

#ifdef NDEBUG
# pragma warning disable 869
#endif

#ifndef _WIN32

#include <stdint.h>
# define override
# define BE_THREAD_LOCAL         __thread
# define BE_NOINLINE             __attribute__((noinline))
# define BE_ALWAYSINLINE         __attribute__((always_inline))
# define BE_SELECTOVERLOAD(o)    

#define BE_SET_ALIGNMENT(n)     __attribute__ ((aligned(n)))
#ifndef BE_STATIC
# define BE_EXPORT              __attribute__ ((visibility("default")))
# define BE_IMPORT
#else
# define BE_EXPORT
# define BE_IMPORT
#endif

#else

# define BE_EXPORT               __declspec(dllexport)
# define BE_IMPORT               __declspec(dllimport)
# define BE_THREAD_LOCAL         __declspec(thread)
# define BE_NOINLINE             __declspec(noinline)
# define BE_ALWAYSINLINE         inline
# define BE_SELECTOVERLOAD(o)    

# define BE_SET_ALIGNMENT(n)     __declspec(align(n))

# pragma warning(disable:4275)
# ifdef NDEBUG
#  pragma warning(error:4541)    // 'dynamic_cast' used on polymorphic type with '/GR-'
#  pragma warning(disable:4530)  // C++ exception handler used, but unwind semantics are not enabled
#  pragma warning(disable:4100)  // unreferenced formal parameter
# endif
# pragma warning(disable:4251)
# pragma warning(disable:4355)   // this used in base member initialization list
# pragma warning(disable:4290)   // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
# pragma warning(disable:4481)   // use of "override" extension
# pragma warning(disable:4127)

# ifndef _CRT_SECURE_NO_WARNINGS
#  define _CRT_SECURE_NO_WARNINGS 1
# endif
# ifndef _CRT_SECURE_NO_DEPRECATE
#  define _CRT_SECURE_NO_DEPRECATE 1
# endif

#endif


/*****************************************************************************/
#endif
