/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_HH_
/*****************************************************************************/
#include    <scheduler/kernel/stream.hh>
#include    <scheduler/task/itask.hh>

namespace BugEngine { namespace Kernel
{

template< typename T >
struct Product
{
public:
    Product(weak< Stream<T> > stream, weak<Task::ITask> producer)
        :   stream(stream)
        ,   producer(producer)
    {
    }
    weak< Stream<T> > const stream;
    weak<Task::ITask> const producer;
};

}}

#define be_product(x)

/*****************************************************************************/
#endif
