/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_HH_
/*****************************************************************************/
#include    <scheduler/kernel/iproduct.script.hh>
#include    <scheduler/kernel/stream.hh>

namespace BugEngine { namespace Kernel
{

template< typename T >
struct Product : public IProduct
{
public:
    Product(weak< Stream<T> > stream, weak<Task::ITask> producer)
        :   IProduct(producer)
        ,   stream(stream)
    {
    }
    weak< Stream<T> > const stream;
};

}}

#define be_product(x)

/*****************************************************************************/
#endif
