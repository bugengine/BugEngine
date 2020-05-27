/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <codeobject.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

KernelObject::KernelObject(weak< const CodeObject > code, const istring name)
    : m_entryPoint(
        code->m_kernel.getSymbol< KernelMain >((minitl::format< 256u >("_%s") | name).c_str()))
{
    be_debug("[%s]: %p" | name | m_entryPoint);
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
