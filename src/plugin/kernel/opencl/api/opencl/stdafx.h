/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_OPENCL_STDAFX_H_
#define BE_KERNEL_OPENCL_STDAFX_H_
/*****************************************************************************/

#include    <bugengine/stdafx.h>

#if defined(building_opencl) || defined(OPENCL_EXPORTS)
# define    OPENCLEXPORT        BE_EXPORT
#elif defined(core_dll)
# define    OPENCLEXPORT        BE_IMPORT
#else
# define    OPENCLEXPORT
#endif

#ifdef BE_PLATFORM_MACOS
# include   <OpenCL/opencl.h>
#else
# include   <CL/opencl.h>
#endif

/*****************************************************************************/
#endif
