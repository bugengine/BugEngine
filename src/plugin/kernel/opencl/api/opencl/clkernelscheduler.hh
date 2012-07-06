/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_OPENCL_CLKERNELSCHEDULER_HH_
#define BE_KERNEL_OPENCL_CLKERNELSCHEDULER_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/ikernelscheduler.hh>
#include    <system/plugin.hh>

namespace BugEngine
{

class OpenCLKernelLoader;

class OpenCLKernelScheduler : public Kernel::IKernelScheduler
{
private:
    weak<ResourceManager>       m_resourceManager;
    scoped<OpenCLKernelLoader>  m_loader;
    cl_context                  m_context;
private:
    static cl_context createCLContextOnPlatform(const cl_context_properties* properties, cl_platform_id platform, cl_device_type deviceType);
    static cl_context createCLContext(const cl_context_properties* properties);
public:
    OpenCLKernelScheduler(const PluginContext& context, const cl_context_properties* properties = 0);
    ~OpenCLKernelScheduler();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}


/*****************************************************************************/
#endif