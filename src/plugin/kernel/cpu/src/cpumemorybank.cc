/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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
