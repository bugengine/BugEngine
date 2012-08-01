/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/kernel/cpumemoryprovider.hh>

namespace BugEngine { namespace Kernel
{

CPUMemoryProvider::CPUMemoryProvider()
    :   IMemoryProvider("CPU")
{
}

CPUMemoryProvider::~CPUMemoryProvider()
{
}

}}
