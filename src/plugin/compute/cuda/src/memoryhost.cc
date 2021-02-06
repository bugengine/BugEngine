/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <memoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

MemoryHost::MemoryHost() : IMemoryHost("Cuda")
{
}

MemoryHost::~MemoryHost()
{
}

void MemoryHost::release(weak< KernelScheduler::IMemoryBuffer > buffer)
{
    be_forceuse(buffer);
}

}}}  // namespace BugEngine::KernelScheduler::Cuda
