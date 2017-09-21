/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/iproduct.script.hh>
#include    <scheduler/kernel/stream.factory.hh>
#include    <rtti/engine/helper/staticarray.hh>
#include    <rtti/typeinfo.hh>

namespace BugEngine { namespace Kernel
{

template< typename T >
struct Product : public IProduct
{
public:
    typedef T Produced;
    Product(weak< Stream<T> > stream, weak<Task::ITask> producer)
        :   IProduct(producer)
        ,   stream(stream)
    {
    }
    weak< Stream<T> > const stream;
};

template< typename PRODUCT >
struct Product_BugHelper
{
    static RTTI::Class s_class;
};

template< typename PRODUCT >
RTTI::Class Product_BugHelper<PRODUCT>::s_class = {
    istring(minitl::format<1024u>("Product<%s>") | be_typeid<typename PRODUCT::Produced>::klass()->name),
    0,
    0,
    RTTI::ClassType_Object,
    {0},
    be_typeid<Kernel::IProduct>::klass(),
    {0},
    {0},
    {0, 0},
    {0, 0},
    {0},
    {0},
    0,
    0
};

}}

namespace BugEngine
{

template< typename T >
struct BE_EXPORT be_typeid< Kernel::Product<T> >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
    }
    static inline raw<const RTTI::Class> klass()
    {
        return ns();
    }
    static inline raw<RTTI::Class> ns()
    {
        raw<RTTI::Class> result = { &Kernel::Product_BugHelper< Kernel::Product<T> >::s_class };
        return result;
    }
};

}

/**************************************************************************************************/
#endif
