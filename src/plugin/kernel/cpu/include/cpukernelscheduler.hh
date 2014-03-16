/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUKERNELSCHEDULER_HH_
#define BE_KERNEL_CPU_CPUKERNELSCHEDULER_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <scheduler/kernel/ikernelscheduler.hh>
#include    <plugin/plugin.hh>

namespace BugEngine
{

class CPUKernelLoader;
class CPUMemoryProvider;

namespace Kernel
{
class IStream;
}


class CPUKernelScheduler : public Kernel::IKernelScheduler
{
private:
    weak<Resource::ResourceManager> m_resourceManager;
    scoped<CPUKernelLoader>         m_loader;
    scoped<CPUMemoryProvider>       m_cpuMemoryProvider;
public:
    CPUKernelScheduler(const Plugin::Context& context);
    ~CPUKernelScheduler();

    virtual void run(weak<const Task::ITask> task, weak<const Kernel::KernelDescription> kernel, const minitl::array<Kernel::KernelParameter>& parameters) override;
    virtual weak<Kernel::IMemoryProvider> memoryProvider() const override;
};

}


/**************************************************************************************************/
#endif
