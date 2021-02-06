/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <memoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

MemoryHost::MemoryHost() : IMemoryHost("CPU")
{
}

MemoryHost::~MemoryHost()
{
}

void MemoryHost::release(weak< KernelScheduler::IMemoryBuffer > buffer)
{
    be_forceuse(buffer);
}

}}}  // namespace BugEngine::KernelScheduler::CPU
