/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_STDAFX_H_
#define BE_COMPUTE_OPENCL_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>

#if defined(building_opencl) || defined(OPENCL_EXPORTS)
# define    OPENCLEXPORT        BE_EXPORT
#else
# define    OPENCLEXPORT        BE_IMPORT
#endif

#ifdef BE_PLATFORM_MACOS
# include   <OpenCL/opencl.h>
#else
# include   <CL/opencl.h>
#endif

/**************************************************************************************************/
#endif
