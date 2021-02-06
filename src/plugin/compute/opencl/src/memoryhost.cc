/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin.compute.opencl/memoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

MemoryHost::MemoryHost() : IMemoryHost("OpenCL")
{
}

MemoryHost::~MemoryHost()
{
}

void MemoryHost::release(weak< KernelScheduler::IMemoryBuffer > buffer)
{
    be_forceuse(buffer);
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
