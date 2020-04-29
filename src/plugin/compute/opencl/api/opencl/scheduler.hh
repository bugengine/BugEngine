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
class KernelObject;
class MemoryHost;


class be_api(OPENCL) Scheduler : public IScheduler
{
    friend class KernelObject;
    typedef minitl::tuple<cl_context, cl_device_id> ClContext;
    typedef minitl::tuple<cl_kernel, cl_program> ClKernel;
private:
    weak<Resource::ResourceManager> m_resourceManager;
    scoped<CodeLoader>              m_loader;
    scoped<MemoryHost>              m_memoryHost;
    ClContext                       m_context;
    int                             m_errorCode;
    cl_command_queue                m_commandQueue;
    const u32                       m_ptrSize;
private:
    static u32 getContextPointerSize(ClContext context);
    static ClContext createCLContextOnPlatform(const cl_context_properties* properties,
                                               cl_platform_id platform,
                                               cl_device_type deviceType);
    static ClContext createCLContext(const cl_context_properties* properties);
private: // friend KernelObject
    ClKernel buildKernel(const size_t size, const unsigned char* byteCode) const;
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
