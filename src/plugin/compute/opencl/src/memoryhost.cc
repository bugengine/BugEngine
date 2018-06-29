/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <opencl/memoryhost.hh>


namespace BugEngine { namespace KernelScheduler { namespace OpenCL
{

MemoryHost::MemoryHost()
    :   IMemoryHost("OpenCL")
{
}

MemoryHost::~MemoryHost()
{
}

}}}
