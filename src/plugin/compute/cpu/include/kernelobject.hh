/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_KERNELOBJECT_HH_
#define BE_COMPUTE_CPU_KERNELOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/plugin/dynobject.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

class Scheduler;

class KernelObject : public minitl::refcountable
{
    friend class Scheduler;

private:
    typedef void(KernelMain)(const u32, const u32,
                             const minitl::array< weak< const IMemoryBuffer > >& params);

private:
    Plugin::DynamicObject m_kernel;
    KernelMain*           m_entryPoint;

public:
    KernelObject(const inamespace& name);
    ~KernelObject();

    void run(const u32 index, const u32 total,
             const minitl::array< weak< const IMemoryBuffer > >& params);
};

}}}  // namespace BugEngine::KernelScheduler::CPU

/**************************************************************************************************/
#endif
