/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_CUDA_MEMORYHOST_HH_
#define BE_COMPUTE_CUDA_MEMORYHOST_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <bugengine/scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

class MemoryHost : public IMemoryHost
{
public:
    MemoryHost();
    ~MemoryHost();

    void release(weak< KernelScheduler::IMemoryBuffer > buffer);
};

}}}  // namespace BugEngine::KernelScheduler::Cuda

/**************************************************************************************************/
#endif
