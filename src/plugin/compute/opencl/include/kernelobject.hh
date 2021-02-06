/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_KERNELOBJECT_HH_
#define BE_COMPUTE_OPENCL_KERNELOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin.compute.opencl/scheduler.hh>
#include <bugengine/plugin/dynobject.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class CodeObject;

class KernelObject : public minitl::refcountable
{
private:
    cl_kernel m_kernel;

public:
    KernelObject(weak< const CodeObject > context, const istring name);
    ~KernelObject();

    void run(const minitl::array< weak< const IMemoryBuffer > >& params);
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
