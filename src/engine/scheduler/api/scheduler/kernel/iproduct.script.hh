/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IPRODUCT_SCRIPT_HH_
#define BE_SCHEDULER_KERNEL_IPRODUCT_SCRIPT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/task/itask.hh>

namespace BugEngine { namespace Kernel
{

struct IProduct
{
public:
    IProduct(weak<Task::ITask> producer)
        :   producer(producer)
    {
    }
    weak<Task::ITask> const producer;
};

}}

/**************************************************************************************************/
#endif
