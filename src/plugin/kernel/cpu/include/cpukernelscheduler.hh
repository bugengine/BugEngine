/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUKERNELSCHEDULER_HH_
#define BE_KERNEL_CPU_CPUKERNELSCHEDULER_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/ikernelscheduler.hh>
#include    <system/plugin.hh>

namespace BugEngine
{

class CpuKernelLoader;

class CpuKernelScheduler : public Kernel::IKernelScheduler
{
private:
    weak<ResourceManager>   m_resourceManager;
    scoped<CpuKernelLoader> m_loader;
public:
    CpuKernelScheduler(const PluginContext& context);
    ~CpuKernelScheduler();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}


/*****************************************************************************/
#endif
