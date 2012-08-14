/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_CPUMEMORYPROVIDER_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_CPUMEMORYPROVIDER_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/imemoryprovider.hh>

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
