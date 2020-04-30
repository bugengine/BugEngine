/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/plugin.compute.cpu/memorybuffer.hh>
#include <memoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

MemoryBuffer::MemoryBuffer(weak< const MemoryHost > provider) : IMemoryBuffer(provider)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

}}}  // namespace BugEngine::KernelScheduler::CPU
