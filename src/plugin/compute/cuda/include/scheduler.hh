/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CUDA_CUDAKERNELSCHEDULER_HH_
#define BE_COMPUTE_CUDA_CUDAKERNELSCHEDULER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

class CodeLoader;
class MemoryHost;
class CUDAKernelTaskItem;

class Scheduler : public IScheduler
{
private:
    weak< Resource::ResourceManager > m_resourceManager;
    ref< CodeLoader >                 m_cudaLoader;
    scoped< MemoryHost >              m_memoryHost;

public:
    Scheduler(const Plugin::Context& context);
    ~Scheduler();

    virtual IKernelTaskItem*    allocateItem(weak< Task::KernelTask > owner,
                                             weak< const Kernel > kernel, u32 parameterCount) override;
    void                        deallocateItem(CUDAKernelTaskItem* item);
    virtual void                run(IKernelTaskItem* item) override;
    virtual weak< IMemoryHost > memoryHost() const override;
};

}}}  // namespace BugEngine::KernelScheduler::Cuda

/**************************************************************************************************/
#endif
