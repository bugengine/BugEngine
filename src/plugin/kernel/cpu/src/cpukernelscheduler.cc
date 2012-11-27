/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpukernelscheduler.hh>
#include    <cpukernelloader.hh>
#include    <cpumemoryprovider.hh>

#include    <kernelobject.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

struct CPUKernelTask
{
    weak<KernelObject>      object;
    KernelObjectParameter   params[16];
    i_u32                   splitCount;

    struct Range
    {
        u32     index;
        i_u32&  total;
        Range(i_u32& taskCount)
            :   index(taskCount)
            ,   total(taskCount)
        {
            taskCount++;
        }
        bool atomic() const { return false; }
        Range split()
        {
            return Range(total);
        }
    };

    CPUKernelTask(weak<KernelObject> object)
        :   object(object)
        ,   splitCount(i_u32::Zero)
    {}

    Range prepare()
    {
        splitCount = i_u32::Zero;
        return Range(splitCount);
    }
    void operator()(const Range& range)
    {
        object->run(range.index, range.total, params);
    }
};

CPUKernelScheduler::CPUKernelScheduler(const Plugin::Context& context)
    :   IKernelScheduler("CPU", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<CPUKernelLoader>::create(Arena::task()))
    ,   m_cpuMemoryProvider(scoped<CPUMemoryProvider>::create(Arena::task()))
{
    m_resourceManager->attach<Kernel::KernelDescription>(m_loader);
}

CPUKernelScheduler::~CPUKernelScheduler()
{
    m_resourceManager->detach<Kernel::KernelDescription>(m_loader);
}

void CPUKernelScheduler::run(weak<const Kernel::KernelDescription> kernel, const minitl::array< weak<Kernel::IStream> >& parameters)
{
    weak<KernelObject> object = kernel->getResource(m_loader).getRefHandle<KernelObject>();
    be_assert(object, "kernel is not loaded");
    for (u32 i = 0; i < parameters.size(); ++i)
    {
    }
}

weak<Kernel::IMemoryProvider> CPUKernelScheduler::memoryProvider() const
{
    return m_cpuMemoryProvider;
}

}
