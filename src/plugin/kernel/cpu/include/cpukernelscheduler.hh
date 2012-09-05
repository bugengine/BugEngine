/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUKERNELSCHEDULER_HH_
#define BE_KERNEL_CPU_CPUKERNELSCHEDULER_HH_
/*****************************************************************************/
#include    <scheduler/kernel/ikernelscheduler.hh>
#include    <plugin/plugin.hh>

namespace BugEngine
{

class CPUKernelLoader;
namespace Kernel
{
class IStream;
}


class CPUKernelScheduler : public Kernel::IKernelScheduler
{
private:
    weak<Resource::ResourceManager> m_resourceManager;
    scoped<CPUKernelLoader>         m_loader;
public:
    CPUKernelScheduler(const PluginContext& context);
    ~CPUKernelScheduler();

    virtual void run(weak<const Kernel::KernelDescription> kernel, const minitl::array< weak<Kernel::IStream> >& parameters) override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}


/*****************************************************************************/
#endif
