/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_FEATURES_HH_
#define BE_MINITL_FEATURES_HH_
/**************************************************************************************************/

#define BE_OPTIM_LEVEL_DEBUG   1
#define BE_OPTIM_LEVEL_PROFILE 2
#define BE_OPTIM_LEVEL_FINAL   3

#ifdef BE_DEBUG
#    define BE_FLAVOUR     Debug
#    define BE_OPTIM_LEVEL BE_OPTIM_LEVEL_DEBUG
#elif defined(BE_PROFILE)
#    define BE_FLAVOUR     Profile
#    define BE_OPTIM_LEVEL BE_OPTIM_LEVEL_PROFILE
#else
#    define BE_FLAVOUR     Final
#    define BE_OPTIM_LEVEL BE_OPTIM_LEVEL_FINAL
#endif

#define BE_OPTIM_LEVEL_AT_MOST(x)  BE_OPTIM_LEVEL <= (x)
#define BE_OPTIM_LEVEL_AT_LEAST(x) BE_OPTIM_LEVEL >= (x)

#if BE_OPTIM_LEVEL_AT_MOST(BE_OPTIM_LEVEL_DEBUG)
#    define BE_ENABLE_ASSERT              1
#    define BE_ENABLE_WEAKCHECK           1
#    define BE_ENABLE_DEBUG_ITERATORS     1
#    define BE_ENABLE_LOGGING             1
#    define BE_ENABLE_MEMORY_TRACKING     1
#    define BE_ENABLE_MEMORY_DEBUGGING    1
#    define BE_ENABLE_EXCEPTIONS          BE_SUPPORTS_EXCEPTIONS
#    define BE_ENABLE_COMPONENT_DEBUGGING 1
#else
#    define BE_ENABLE_ASSERT              0
#    define BE_ENABLE_WEAKCHECK           0
#    define BE_ENABLE_DEBUG_ITERATORS     0
#    define BE_ENABLE_LOGGING             0
#    define BE_ENABLE_MEMORY_TRACKING     0
#    define BE_ENABLE_MEMORY_DEBUGGING    0
#    define BE_ENABLE_EXCEPTIONS          0
#    define BE_ENABLE_COMPONENT_DEBUGGING 0
#endif

/**************************************************************************************************/
#endif
