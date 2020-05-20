/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_CONTEXT_HH_
#define BE_COMPUTE_OPENCL_CONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.opencl/stdafx.h>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class Platform;
class Scheduler;

class Context : public minitl::refcountable
{
    friend class Scheduler;

private:
    const weak< Platform > m_platform;
    const cl_device_id     m_device;
    const cl_context       m_context;
    const u32              m_pointerSize;

public:
    Context(weak< Platform > platform, cl_device_id device, cl_context context);
    ~Context();

    inline u32 getPointerSize() const
    {
        return m_pointerSize;
    }

    cl_program buildProgram(const u64 size, const char* code) const;
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
