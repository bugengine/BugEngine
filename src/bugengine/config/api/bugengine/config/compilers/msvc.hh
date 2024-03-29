/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_COMPILERS_MSVC_HH_
#define BE_CONFIG_COMPILERS_MSVC_HH_
/**************************************************************************************************/

#if(_MSC_VER >= 1400) && !defined(_ARM) && !defined(_ARM64)
//  Following 8 lines: workaround for a bug in some older SDKs
#    pragma push_macro("_interlockedbittestandset")
#    pragma push_macro("_interlockedbittestandreset")
#    pragma push_macro("_interlockedbittestandset64")
#    pragma push_macro("_interlockedbittestandreset64")
#    define _interlockedbittestandset     _local_interlockedbittestandset
#    define _interlockedbittestandreset   _local_interlockedbittestandreset
#    define _interlockedbittestandset64   _local_interlockedbittestandset64
#    define _interlockedbittestandreset64 _local_interlockedbittestandreset64
#    include <intrin.h>  // to force the header not to be included elsewhere
#    pragma pop_macro("_interlockedbittestandreset64")
#    pragma pop_macro("_interlockedbittestandset64")
#    pragma pop_macro("_interlockedbittestandreset")
#    pragma pop_macro("_interlockedbittestandset")
#endif

#ifndef __cplusplus
#    define be_restrict restrict
#else
#    define be_restrict __restrict
#endif

#define be_alignof(t) __alignof(t&)

typedef signed __int8    i8;
typedef signed __int16   i16;
typedef signed __int32   i32;
typedef signed __int64   i64;
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef u8               byte;

// member function does not override any base class virtual member function
#pragma warning(default : 4263)
// no override available for virtual member function from base 'class'; function is hidden
#pragma warning(default : 4264)
// class has virtual functions, but destructor is not virtual
#pragma warning(default : 4265)
// no override available for virtual member function from base'type'; function is hidden
#if(_MSC_VER < 1400)
#    pragma warning(disable : 4266)
#else
#    pragma warning(default : 4266)
#endif
// wide string literal cast to 'LPSTR'
#pragma warning(default : 4906)
// reinterpret_cast used between related classes
#pragma warning(default : 4946)
// not all control paths return a value
#pragma warning(error : 4715)

// class 'X' needs to have dll-interface to be used by clients of class 'Y'
#pragma warning(disable : 4251)
// non dll-interface class 'X' used as base for dll-interface class 'Y'
#pragma warning(disable : 4275)
#ifdef NDEBUG
// 'dynamic_cast' used on polymorphic type with '/GR-'
#    pragma warning(error : 4541)
// C++ exception handler used, but unwind semantics are not enabled
#    pragma warning(disable : 4530)
// unreferenced formal parameter
#    pragma warning(disable : 4100)
// unreachable code
#    pragma warning(disable : 4702)
#endif
// new behavior: elements of array '...' will be default initialized
#pragma warning(disable : 4351)
// this used in base member initialization list
#pragma warning(disable : 4355)
// conditional expression is constant
#pragma warning(disable : 4127)
// unreferenced local function has been removed
#pragma warning(disable : 4505)
// default constructor could not be generated
#pragma warning(disable : 4510)
// copy constructor could not be generated
#pragma warning(disable : 4511)
// assignment operator could not be generated
#pragma warning(disable : 4512)
// struct X can never be instantiated - user defined constructor required
#pragma warning(disable : 4610)
// forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable : 4800)
// Alignment specifier is less than actual alignment (X) and will be ignored
#pragma warning(disable : 4359)
// structure was padded due to alignment specifier
#pragma warning(disable : 4324)

#define BE_NOINLINE     __declspec(noinline)
#define BE_ALWAYSINLINE __forceinline
#ifdef _CPPUNWIND
#    define BE_SUPPORTS_EXCEPTIONS 1
#else
#    define BE_SUPPORTS_EXCEPTIONS 0
#endif

#ifndef BE_STATIC
#    define BE_EXPORT __declspec(dllexport)
#    define BE_IMPORT __declspec(dllimport)
#else
#    define BE_EXPORT
#    define BE_IMPORT
#endif

#if _MSC_VER >= 1600
#    define BE_HAS_MOVE_SEMANTICS
#endif

#if _MSC_VER >= 1300
/*# ifndef _XBOX
#  include <xmmintrin.h>
# endif*/
#    define be_break() __debugbreak()
#else
#    define be_break() asm { int 3;}
#endif

#if _MSC_VER >= 1400
// use of "override" extension
#    pragma warning(disable : 4481)
#else
#    define override
// unreachable code
#    pragma warning(disable : 4702)
// function marked as __forceinline but not inlined
#    pragma warning(disable : 4714)
#endif

#define _CRT_SECURE_NO_DEPRECATE 1

/**************************************************************************************************/
#endif
