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

#ifdef BE_PLATFORM_MACOS
#    include <OpenCL/opencl.h>
#else
#    include <CL/cl.h>
#endif

#define checkResult(a)                                                                             \
    do                                                                                             \
    {                                                                                              \
        cl_int err = a;                                                                            \
        if(err != CL_SUCCESS) be_error("OpenCL call %s failed with error code %d" | #a | err);     \
    } while(0)

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

struct CLStringInfo
{
    enum
    {
        InfoLogSize = 1024
    };
    char info[InfoLogSize];
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
