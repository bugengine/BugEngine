/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_CODEOBJECT_HH_
#define BE_COMPUTE_OPENCL_CODEOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin/dynobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class Context;
class KernelObject;

class CodeObject : public minitl::refcountable
{
    friend class KernelObject;

private:
    Plugin::DynamicObject m_kernel;
    cl_program            m_program;

public:
    CodeObject(weak< const Context > context, const inamespace& name);
    ~CodeObject();
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
