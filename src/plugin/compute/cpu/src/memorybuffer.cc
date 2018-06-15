/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpu/memorybuffer.hh>
#include    <memoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU
{

MemoryBuffer::MemoryBuffer(weak<const MemoryHost> provider)
    :   IMemoryBuffer(provider)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

}}}
