/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_KERNELOBJECT_HH_
#define BE_COMPUTE_CPU_KERNELOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cpu/stdafx.h>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

class CodeObject;
class Scheduler;

class KernelObject : public minitl::refcountable
{
    friend class Scheduler;

private:
    typedef void(KernelMain)(const u32, const u32,
                             const minitl::array< weak< const IMemoryBuffer > >& params);

private:
    KernelMain* m_entryPoint;

public:
    KernelObject(weak< const CodeObject > code, const istring name);
    ~KernelObject();

    void run(const u32 index, const u32 total,
             const minitl::array< weak< const IMemoryBuffer > >& params);
};

}}}  // namespace BugEngine::KernelScheduler::CPU

/**************************************************************************************************/
#endif
