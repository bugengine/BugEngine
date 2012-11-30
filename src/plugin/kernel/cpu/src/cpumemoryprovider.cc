/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpumemoryprovider.hh>

namespace BugEngine
{

CPUMemoryProvider::CPUMemoryProvider()
    :   Kernel::IMemoryProvider("CPU")
{
}

CPUMemoryProvider::~CPUMemoryProvider()
{
}

Kernel::KernelParameter CPUMemoryProvider::getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const
{
    be_forceuse(bank);
    Kernel::KernelParameter parameter = { 0, 0 };
    return parameter;
}

}
