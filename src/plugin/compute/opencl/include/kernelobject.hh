/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_KERNELOBJECT_HH_
#define BE_COMPUTE_OPENCL_KERNELOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin.compute.opencl/scheduler.hh>
#include <bugengine/plugin/dynobject.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class Context;

class KernelObject : public minitl::refcountable
{
private:
    typedef const char* KernelBlob;

private:
    Plugin::DynamicObject m_kernel;
    KernelBlob            m_kernelBlob;
    cl_program            m_program;

public:
    KernelObject(weak< const Context > context, const inamespace& name);
    ~KernelObject();

    void run(const minitl::array< weak< const IMemoryBuffer > >& params);
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
