/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace KernelScheduler {

Kernel::Kernel(ref< const Code > code, const istring& name) : m_kernelCode(code), m_name(name)
{
}

Kernel::~Kernel()
{
}

}}  // namespace BugEngine::KernelScheduler
