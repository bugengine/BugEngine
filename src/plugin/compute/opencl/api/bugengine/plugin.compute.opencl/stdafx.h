/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_STDAFX_H_
#define BE_COMPUTE_OPENCL_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/stdafx.h>

#if defined(building_opencl)
#    define BE_API_OPENCL BE_EXPORT
#elif defined(be_dll_opencl)
#    define BE_API_OPENCL BE_IMPORT
#else
#    define BE_API_OPENCL
#endif

#define CL_USE_DEPRECATED_OPENCL_1_0_APIS 1
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS 1
#ifdef BE_PLATFORM_MACOS
#    include <OpenCL/opencl.h>
#else
#    include <CL/opencl.h>
#endif

/**************************************************************************************************/
#endif
