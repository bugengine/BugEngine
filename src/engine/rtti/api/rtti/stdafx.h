/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_STDAFX_H_
#define BE_RTTI_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/debug/logger.hh>
#include    <maths/vector.hh>
#include    <maths/matrix.hh>


#if defined(building_rtti) || defined(RTTI_EXPORTS)
# define     RTTIEXPORT          BE_EXPORT
#elif defined(rtti_dll)
# define     RTTIEXPORT          BE_IMPORT
#else
# define     RTTIEXPORT
#endif

#define     published           public
#define     be_tag(v)

/*****************************************************************************/
#endif
