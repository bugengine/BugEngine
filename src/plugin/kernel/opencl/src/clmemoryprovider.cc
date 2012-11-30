/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
