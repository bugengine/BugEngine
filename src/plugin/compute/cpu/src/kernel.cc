/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

KernelObject::KernelObject(const inamespace& name)
    : m_kernel(name, ipath("kernel"))
    , m_entryPoint(m_kernel.getSymbol< KernelMain >("_kmain"))
{
    be_debug("kernel entry point: %p" | m_entryPoint);
}

KernelObject::~KernelObject()
{
}

void KernelObject::run(const u32 index, const u32 total,
                       const minitl::array< weak< const IMemoryBuffer > >& params)
{
    if(m_entryPoint) (*m_entryPoint)(index, total, params);
}

}}}  // namespace BugEngine::KernelScheduler::CPU
