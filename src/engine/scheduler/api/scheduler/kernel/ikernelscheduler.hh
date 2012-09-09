/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
#define BE_SCHEDULER_KERNEL_IKERNELSCHEDULER_HH_
/*****************************************************************************/
#include    <minitl/array.hh>

namespace BugEngine
{

class Scheduler;

namespace Kernel
{

class IMemoryProvider;
class IStream;
class KernelDescription;

class be_api(SCHEDULER) IKernelScheduler : public minitl::pointer
{
    friend class ::BugEngine::Scheduler;
private:
    istring const               m_name;
    weak<Scheduler> const       m_scheduler;
    weak<IMemoryProvider> const m_memoryProvider;
public:
    IKernelScheduler(const istring& name, weak<Scheduler> scheduler, weak<IMemoryProvider> provider);
    ~IKernelScheduler();

    virtual void run(weak<const Kernel::KernelDescription> kernel, const minitl::array< weak<Kernel::IStream> >& parameters) = 0;
};

}}

/*****************************************************************************/
#endif
