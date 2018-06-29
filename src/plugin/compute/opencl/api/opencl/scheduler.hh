/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_SCHEDULER_HH_
#define BE_COMPUTE_OPENCL_SCHEDULER_HH_
/**************************************************************************************************/
#include    <opencl/stdafx.h>
#include    <scheduler/kernel/ischeduler.hh>
#include    <plugin/plugin.hh>


namespace BugEngine { namespace KernelScheduler { namespace OpenCL
{

class CodeLoader;
class MemoryHost;

class be_api(OPENCL) Scheduler : public IScheduler
{
private:
    weak<Resource::ResourceManager> m_resourceManager;
    scoped<CodeLoader>              m_loader;
    scoped<MemoryHost>              m_memoryHost;
    cl_context                      m_context;
private:
    static cl_context createCLContextOnPlatform(const cl_context_properties* properties,
                                                cl_platform_id platform,
                                                cl_device_type deviceType);
    static cl_context createCLContext(const cl_context_properties* properties);
public:
    Scheduler(const Plugin::Context& context, const cl_context_properties* properties = 0);
    ~Scheduler();
    virtual void run(weak<Task::KernelTask> task,
                     weak<const Kernel> kernel,
                     const minitl::array< weak<const IMemoryBuffer> >& parameters) override;
    virtual weak<IMemoryHost> memoryHost() const override;
};

}}}


/**************************************************************************************************/
#endif
