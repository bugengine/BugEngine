/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_COMPILERS_INTEL_HH_
#define BE_CONFIG_COMPILERS_INTEL_HH_
/**************************************************************************************************/

#define be_alignof(t) __alignof(t)
#define be_break()    /*__asm("int $3")*/

#pragma warning disable 111  // statement is unreachable
#pragma warning disable 161  // unknown pragma
#pragma warning disable 193  // zero used for undefined preprocessing identifier
#pragma warning disable 279  // controlling expression is constant
#pragma warning disable 383  // reference to copy
#pragma warning disable 424  // extra ;
#pragma warning disable 177  // variable was declared but never used
#pragma warning disable 593  // variable was set but never used
#pragma warning disable 981
#pragma warning disable 1418  // external function definition with no prior declaration
#pragma warning disable 2259
#pragma warning disable 411   // class X defines no constructor to initialize...
#pragma warning disable 2304  // explicit constructors
#pragma warning disable 2289  // signature for copy is Type(const Type&)
#pragma warning disable 444   // destructor for base class is not virtual
#pragma warning disable 810   // conversion from X to Y may lose significant bits
#pragma warning disable 1572  // floating-point equality and inequality comparisons are unreliable
#pragma warning disable 1419  // external declaration in primary source file
#pragma warning disable 1740  // dllexport/dllimport conflict with "XXX" (declared at line YY);
                              // dllexport assumed
#pragma warning disable 1292  // unknown attribute "xxx"
#pragma warning                                                                                    \
    disable 654  // overloaded virtual function "X:xxx" is only partially overridden in class "Y"

#if __ICL >= 1200
#    pragma warning disable 791   // calling convention specified more than once
#    pragma warning disable 1028  // unknown warning specifier
#    pragma warning disable 1879  // unimplemented pragma ignored
#    pragma warning disable 2586  // decorated name length exceeded, name was truncated
#    pragma warning disable 2659  // attribute "align" ignored because no definition follows
#endif
#if __ICL >= 1300
#    pragma warning disable 864   //  extern inline function "XXX" was referenced but not defined
#    pragma warning disable 1885  // #pragma region unclosed at end of file
#    pragma warning                                                                                \
        disable 3199  // "defined" is always false in a macro expansion in Microsoft mode
#endif
#ifdef NDEBUG
#    pragma warning disable 869
#endif

#ifndef __cplusplus
#    define be_restrict restrict
#else
#    define be_restrict __restrict
#endif

#ifndef _WIN32

#    include <stdint.h>
#    include <unistd.h>
typedef signed __int8    i8;
typedef signed __int16   i16;
typedef signed __int32   i32;
typedef signed __int64   i64;
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef u8               byte;

#    define override
#    define BE_NOINLINE     __attribute__((noinline))
#    define BE_ALWAYSINLINE __attribute__((always_inline))

#    ifndef BE_STATIC
#        define BE_EXPORT __attribute__((visibility("default")))
#        define BE_IMPORT
#    else
#        define BE_EXPORT
#        define BE_IMPORT
#    endif
#    ifdef __EXCEPTIONS
#        define BE_SUPPORTS_EXCEPTIONS 1
#    else
#        define BE_SUPPORTS_EXCEPTIONS 0
#    endif
#else

typedef signed __int8    i8;
typedef signed __int16   i16;
typedef signed __int32   i32;
typedef signed __int64   i64;
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef u8               byte;

#    define BE_EXPORT       __declspec(dllexport)
#    define BE_IMPORT       __declspec(dllimport)
#    define BE_NOINLINE     __declspec(noinline)
#    define BE_ALWAYSINLINE inline
#    ifdef _CPPUNWIND
#        define BE_SUPPORTS_EXCEPTIONS 1
#    else
#        define BE_SUPPORTS_EXCEPTIONS 0
#    endif

#    pragma warning(disable : 4275)
#    ifdef NDEBUG
#        pragma warning(error : 4541)  // 'dynamic_cast' used on polymorphic type with '/GR-'
#        pragma warning(                                                                           \
            disable : 4530)  // C++ exception handler used, but unwind semantics are not enabled
#        pragma warning(disable : 4100)  // unreferenced formal parameter
#    endif
#    pragma warning(disable : 4251)
#    pragma warning(disable : 4355)  // this used in base member initialization list
#    pragma warning(                                                                               \
        disable : 4290)  // C++ exception specification ignored except to indicate a function is not
                         // __declspec(nothrow)
#    pragma warning(disable : 4127)

#    if __ICL < 1000
#        define override
#    elif _MSC_VER >= 1400
#        pragma warning(disable : 4481)  // use of "override" extension
#    else
#        define override
#    endif

#    ifndef _CRT_SECURE_NO_WARNINGS
#        define _CRT_SECURE_NO_WARNINGS 1
#    endif
#    ifndef _CRT_SECURE_NO_DEPRECATE
#        define _CRT_SECURE_NO_DEPRECATE 1
#    endif

#endif

/**************************************************************************************************/
#endif
