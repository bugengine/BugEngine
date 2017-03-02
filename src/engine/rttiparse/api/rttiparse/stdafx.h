/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_STDAFX_H_
#define BE_RTTIPARSE_STDAFX_H_
/**************************************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <filesystem/stdafx.h>


#if defined(building_rttiparse) || defined(RTTIPARSE_EXPORTS)
# define     RTTIPARSEEXPORT     BE_EXPORT
#else
# define     RTTIPARSEEXPORT     BE_IMPORT
#endif

/**************************************************************************************************/
#endif
