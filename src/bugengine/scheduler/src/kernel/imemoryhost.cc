/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace KernelScheduler {

IMemoryHost::IMemoryHost(const istring& name) : m_name(name)
{
}

IMemoryHost::~IMemoryHost()
{
}

}}  // namespace BugEngine::KernelScheduler
