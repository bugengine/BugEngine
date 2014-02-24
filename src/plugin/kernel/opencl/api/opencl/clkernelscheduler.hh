/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_OPENCL_CLKERNELSCHEDULER_HH_
#define BE_KERNEL_OPENCL_CLKERNELSCHEDULER_HH_
/**************************************************************************************************/
#include    <scheduler/kernel/ikernelscheduler.hh>
#include    <plugin/plugin.hh>

namespace BugEngine
{

class OpenCLKernelLoader;
class OpenCLMemoryProvider;

class be_api(OPENCL) OpenCLKernelScheduler : public Kernel::IKernelScheduler
{
private:
    weak<Resource::ResourceManager> m_resourceManager;
    scoped<OpenCLKernelLoader>      m_loader;
    scoped<OpenCLMemoryProvider>    m_memoryProvider;
    cl_context                      m_context;
private:
    static cl_context createCLContextOnPlatform(const cl_context_properties* properties, cl_platform_id platform, cl_device_type deviceType);
    static cl_context createCLContext(const cl_context_properties* properties);
public:
    OpenCLKernelScheduler(const Plugin::Context& context, const cl_context_properties* properties = 0);
    ~OpenCLKernelScheduler();
    virtual void run(weak<const Task::ITask> task, weak<const Kernel::KernelDescription> kernel, const minitl::array<Kernel::KernelParameter>& parameters) override;
    virtual weak<Kernel::IMemoryProvider> memoryProvider() const override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}


/**************************************************************************************************/
#endif
