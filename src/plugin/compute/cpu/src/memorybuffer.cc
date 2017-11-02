/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <memorybuffer.hh>
#include    <memoryhost.hh>

namespace BugEngine { namespace Kernel { namespace CPU
{

MemoryBuffer::MemoryBuffer(weak<const MemoryHost> provider)
    :   IMemoryBuffer(provider)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

}}}
