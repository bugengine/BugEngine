/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <bugengine/plugin.compute.cuda/memorybuffer.hh>
#include <memoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

MemoryBuffer::MemoryBuffer(weak< const MemoryHost > provider) : IMemoryBuffer(provider)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

}}}  // namespace BugEngine::KernelScheduler::Cuda
