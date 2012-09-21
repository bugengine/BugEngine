/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/cpumemorybank.hh>
#include    <scheduler/kernel/cpumemoryprovider.hh>

namespace BugEngine { namespace Kernel
{

CPUMemoryBank::CPUMemoryBank(weak<const CPUMemoryProvider> provider)
    :   IMemoryBank(provider)
{
}

CPUMemoryBank::~CPUMemoryBank()
{
}

}}
