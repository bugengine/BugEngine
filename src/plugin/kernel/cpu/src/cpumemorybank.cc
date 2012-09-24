/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cpumemorybank.hh>
#include    <cpumemoryprovider.hh>

namespace BugEngine
{

CPUMemoryBank::CPUMemoryBank(weak<const CPUMemoryProvider> provider)
    :   IMemoryBank(provider)
{
}

CPUMemoryBank::~CPUMemoryBank()
{
}

}
