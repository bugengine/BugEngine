/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_MEMORYHOST_HH_
#define BE_COMPUTE_OPENCL_MEMORYHOST_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class MemoryHost : public IMemoryHost
{
public:
    MemoryHost();
    ~MemoryHost();

    void release(weak< KernelScheduler::IMemoryBuffer > buffer);
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
