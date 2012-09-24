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

}
