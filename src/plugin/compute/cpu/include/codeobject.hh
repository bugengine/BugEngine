/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_CODEOBJECT_HH_
#define BE_COMPUTE_CPU_CODEOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/plugin/dynobject.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

class KernelObject;

class CodeObject : public minitl::refcountable
{
    friend class KernelObject;

private:
    Plugin::DynamicObject m_kernel;

public:
    CodeObject(const inamespace& name);
    ~CodeObject();
};

}}}  // namespace BugEngine::KernelScheduler::CPU

/**************************************************************************************************/
#endif
