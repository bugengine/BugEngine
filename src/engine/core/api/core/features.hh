/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_FEATURES_H_
#define BE_CORE_FEATURES_H_
/*****************************************************************************/

#define     BE_OPTIM_LEVEL_DEBUG    1
#define     BE_OPTIM_LEVEL_FINAL    2

#ifdef _DEBUG
# define    BE_DEBUG        1
# define    BE_FLAVOUR      Debug
# define    BE_OPTIM_LEVEL  BE_OPTIM_LEVEL_DEBUG
#else
# define    BE_FINAL        1
# define    BE_FLAVOUR      Final
# define    BE_OPTIM_LEVEL  BE_OPTIM_LEVEL_FINAL
#endif

#define     BE_OPTIM_LEVEL_AT_MOST(x)       BE_OPTIM_LEVEL <= (x)
#define     BE_OPTIM_LEVEL_AT_LEAST(x)      BE_OPTIM_LEVEL >= (x)


#if BE_OPTIM_LEVEL_AT_MOST(BE_OPTIM_LEVEL_DEBUG)
# define BE_ENABLE_ASSERT               1
# define BE_ENABLE_WEAKCHECK            1
# define BE_ENABLE_DEBUG_ITERATORS      1
# define BE_ENABLE_LOGGING              1
# define BE_ENABLE_MEMORY_TRACKING      1
# define BE_ENABLE_EXCEPTIONS           1
#endif

/*****************************************************************************/
#endif
