/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_SCHEDULER_HH_
#define BE_COMPUTE_OPENCL_SCHEDULER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/scheduler/kernel/ischeduler.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class Context;
class KernelLoader;
class MemoryHost;
class CLKernelTaskItem;

class be_api(OPENCL) Scheduler : public IScheduler
{
private:
    ref< Context >                    m_context;
    weak< Resource::ResourceManager > m_resourceManager;
    scoped< KernelLoader >            m_loader;
    scoped< MemoryHost >              m_memoryHost;
    int                               m_errorCode;
    cl_command_queue                  m_commandQueue;

public:
    Scheduler(const Plugin::Context& pluginContext, ref< Context > clContext);
    ~Scheduler();
    virtual IKernelTaskItem*    allocateItem(weak< Task::KernelTask > owner,
                                             weak< const Kernel > kernel, u32 parameterCount) override;
    void                        deallocateItem(CLKernelTaskItem * item);
    virtual void                run(IKernelTaskItem * item) override;
    virtual weak< IMemoryHost > memoryHost() const override;
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
