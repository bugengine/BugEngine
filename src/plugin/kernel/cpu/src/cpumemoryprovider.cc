/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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
