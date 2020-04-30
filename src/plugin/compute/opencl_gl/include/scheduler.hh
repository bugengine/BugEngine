/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_OPENGL_SCHEDULER_HH_
#define BE_COMPUTE_OPENCL_OPENGL_SCHEDULER_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/plugin.compute.opencl/scheduler.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL_GL {

class Scheduler : public OpenCL::Scheduler
{
private:
    static minitl::array< cl_context_properties > createPlatformSpecificContextProperties();

public:
    Scheduler(const Plugin::Context& context);
    ~Scheduler();

public:
    void* operator new(size_t size, void* where)
    {
        return ::operator new(size, where);
    }
    void operator delete(void* memory, void* where)
    {
        ::operator delete(memory, where);
    }
    void operator delete(void* memory)
    {
        be_notreached();
        ::operator delete(memory);
    }
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL_GL

/**************************************************************************************************/
#endif
