/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_CPUMEMORYPROVIDER_HH_
#define BE_SCHEDULER_KERNEL_CPUMEMORYPROVIDER_HH_
/*****************************************************************************/
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine { namespace Kernel
{

class CPUMemoryProvider : public IMemoryProvider
{
public:
    CPUMemoryProvider();
    ~CPUMemoryProvider();
};

}}

/*****************************************************************************/
#endif
