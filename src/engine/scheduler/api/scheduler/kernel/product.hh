/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/iproduct.script.hh>


namespace BugEngine { namespace Kernel
{

template< typename T >
class Product : public IProduct
{
public:
    Product(ref<T> parameter, weak<Task::ITask> producer)
        :   IProduct(parameter, producer)
    {
    }

    Product(weak<const Product> other, weak<Task::ITask> producer)
        :   IProduct(other->m_parameter, producer)
    {
    }

    ~Product()
    {
    }

public:
    weak<T> parameter() const
    {
        return be_checked_cast<T>(m_parameter);
    }
};

}}

#include    <scheduler/kernel/product.factory.hh>

/**************************************************************************************************/
#endif
