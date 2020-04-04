/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <cuda/stdafx.h>
#include    <cuda/memorybuffer.hh>
#include    <memoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda
{

MemoryBuffer::MemoryBuffer(weak<const MemoryHost> provider)
    :   IMemoryBuffer(provider)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

}}}
