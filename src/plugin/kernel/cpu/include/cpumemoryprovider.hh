/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_CPUMEMORYPROVIDER_HH_
#define BE_KERNEL_CPU_CPUMEMORYPROVIDER_HH_
/*****************************************************************************/
#include    <scheduler/kernel/imemoryprovider.hh>
#include    <scheduler/kernel/istream.hh>

namespace BugEngine
{

class CPUMemoryProvider : public Kernel::IMemoryProvider
{
public:
    CPUMemoryProvider();
    ~CPUMemoryProvider();
};

}

/*****************************************************************************/
#endif