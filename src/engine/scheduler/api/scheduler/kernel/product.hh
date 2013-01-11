/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_HH_
/*****************************************************************************/
#include    <scheduler/kernel/iproduct.script.hh>
#include    <scheduler/kernel/stream.hh>
#include    <rtti/typeinfo.hh>

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
        static const RTTI::Class s_class =
        {
            istring(minitl::format<1024u>("Product<%s>") | be_typeid<T>::klass()->name),
            {0},
            be_typeid<Kernel::IProduct>::klass(),
            0,
            0,
            {0},
            {0},
            {0},
            {0},
            {0},
            {0},
            0,
            0
        };
        raw<const RTTI::Class> result = { &s_class };
        return result;
    }
};

}

/*****************************************************************************/
#endif
