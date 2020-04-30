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

class Scheduler : public IScheduler
{
private:
    weak< Resource::ResourceManager > m_resourceManager;
    ref< CodeLoader >                 m_cudaLoader;
    scoped< MemoryHost >              m_memoryHost;

public:
    Scheduler(const Plugin::Context& context);
    ~Scheduler();

    virtual void                run(weak< Task::KernelTask > task, weak< const Kernel > kernel,
                                    const minitl::array< weak< const IMemoryBuffer > >& parameters) override;
    virtual weak< IMemoryHost > memoryHost() const override;
};

}}}  // namespace BugEngine::KernelScheduler::Cuda

/**************************************************************************************************/
#endif
