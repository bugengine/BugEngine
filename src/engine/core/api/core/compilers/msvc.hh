/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_COMPILERS_MSVC_H_
#define BE_CORE_COMPILERS_MSVC_H_
/*****************************************************************************/

#define be_alignof(t)          __alignof(t)

typedef signed __int8           i8;
typedef signed __int16          i16;
typedef signed __int32          i32;
typedef signed __int64          i64;
typedef unsigned __int8         u8;
typedef unsigned __int16        u16;
typedef unsigned __int32        u32;
typedef unsigned __int64        u64;
typedef u8                      byte;

#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS 1
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE 1
#endif

#pragma warning(disable:4275)
#ifdef NDEBUG
# pragma warning(error:4541)   // 'dynamic_cast' used on polymorphic type with '/GR-'
# pragma warning(disable:4530) // C++ exception handler used, but unwind semantics are not enabled
# pragma warning(disable:4100) // unreferenced formal parameter
#endif
#pragma warning(disable:4251)
#pragma warning(disable:4355)  // this used in base member initialization list
#pragma warning(disable:4290)  // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning(disable:4481)  // use of "override" extension
#pragma warning(disable:4127)
#pragma warning(disable:4709)  // comma operator used in []
#define BE_THREAD_LOCAL        __declspec(thread)
#define BE_NOINLINE            __declspec(noinline)
#define BE_ALWAYSINLINE        __forceinline
#define NOTHROW     throw()

#define BE_SET_ALIGNMENT(n)     __declspec(align(n)) 
#define BE_EXPORT               __declspec(dllexport)
#define BE_IMPORT               __declspec(dllimport)

/*****************************************************************************/
#endif
