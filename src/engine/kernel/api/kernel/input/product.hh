/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/iproduct.script.hh>

namespace BugEngine { namespace Kernel
{

template< typename PARAMETER_TYPE >
class Product : public IProduct
{
public:
    inline Product(weak<PARAMETER_TYPE> parameter, weak<Task::ITask> producer);
    inline ~Product();

    inline weak<PARAMETER_TYPE> getParameter() const;
};


template< typename PARAMETER_TYPE >
Product<PARAMETER_TYPE>::Product(weak<PARAMETER_TYPE> parameter, weak<Task::ITask> producer)
    :   IProduct(parameter, producer)
{
}

template< typename PARAMETER_TYPE >
Product<PARAMETER_TYPE>::~Product()
{
}

template< typename PARAMETER_TYPE >
weak<PARAMETER_TYPE> Product<PARAMETER_TYPE>::getParameter() const
{
    return be_checked_cast<PARAMETER_TYPE>(m_parameter);
}

}}

#include    <scheduler/kernel/product.factory.hh>

/**************************************************************************************************/
#endif
