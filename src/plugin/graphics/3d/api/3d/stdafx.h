/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_STDAFX_H_
#define BE_3D_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>


#if defined(building_3d)
# define    BE_API_3D      BE_EXPORT
#elif defined(be_dll_3d)
# define    BE_API_3D      BE_IMPORT
#else
# define    BE_API_3D
#endif

/**************************************************************************************************/
#endif
