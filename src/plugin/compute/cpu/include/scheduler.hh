/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_CPUKERNELSCHEDULER_HH_
#define BE_COMPUTE_CPU_CPUKERNELSCHEDULER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

class CodeLoader;
class MemoryHost;
class CPUKernelTaskItem;

class Scheduler : public IScheduler
{
private:
    weak< Resource::ResourceManager >   m_resourceManager;
    minitl::vector< ref< CodeLoader > > m_cpuLoaders;
    scoped< MemoryHost >                m_memoryHost;

public:
    Scheduler(const Plugin::Context& context);
    ~Scheduler();

    virtual IKernelTaskItem*    allocateItem(weak< Task::KernelTask > owner,
                                             weak< const Kernel > kernel, u32 parameterCount) override;
    void                        deallocateItem(CPUKernelTaskItem* item);
    virtual void                run(IKernelTaskItem* item) override;
    virtual weak< IMemoryHost > memoryHost() const override;
};

}}}  // namespace BugEngine::KernelScheduler::CPU

/**************************************************************************************************/
#endif
