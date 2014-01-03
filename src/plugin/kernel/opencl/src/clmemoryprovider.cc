/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <opencl/clmemoryprovider.hh>

namespace BugEngine
{

OpenCLMemoryProvider::OpenCLMemoryProvider()
    :   IMemoryProvider("OpenCL")
{
}

OpenCLMemoryProvider::~OpenCLMemoryProvider()
{
}

Kernel::KernelParameter OpenCLMemoryProvider::getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const
{
    be_forceuse(bank);
    Kernel::KernelParameter parameter = { 0, 0 };
    return parameter;
}

}
