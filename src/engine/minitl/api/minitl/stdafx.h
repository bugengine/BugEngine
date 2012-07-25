/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_STDAFX_H_
#define BE_MINITL_STDAFX_H_
/*****************************************************************************/

#include    <kernel/stdafx.h>
#include    <minitl/features.hh>

#define     BE_NOCOPY(x)        private: x(const x&); x& operator=(const x&);
#define     be_api(module) module##EXPORT

#if defined(building_minitl) || defined(MINITL_EXPORTS)
# define    MINITLEXPORT         BE_EXPORT
#elif defined(minitl_dll)
# define    MINITLEXPORT         BE_IMPORT
#else
# define    MINITLEXPORT
#endif

/*****************************************************************************/
#endif
