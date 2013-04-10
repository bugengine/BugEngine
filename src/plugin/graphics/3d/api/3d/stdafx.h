/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_STDAFX_H_
#define BE_3D_STDAFX_H_
/*****************************************************************************/

#include    <bugengine/stdafx.h>


#if defined(building_3d)
# define    _3DEXPORT      BE_EXPORT
#else
# define    _3DEXPORT      BE_IMPORT
#endif

/*****************************************************************************/
#endif
