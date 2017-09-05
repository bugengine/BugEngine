/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/iproduct.script.hh>
#include    <scheduler/kernel/stream.factory.hh>
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
        return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
    }
    static inline raw<RTTI::Class> preklass()
    {
        static RTTI::Class s_class =
        {
            istring(minitl::format<1024u>("Product<%s>") | be_typeid<T>::klass()->name),
            0,
            0,
            RTTI::ClassType_Object,
            {0},
            be_typeid<Kernel::IProduct>::klass(),
            {0},
            {&RTTI::staticarray<const RTTI::Tag>::s_null},
            {&RTTI::staticarray<const RTTI::Property>::s_null},
            {&RTTI::staticarray<const RTTI::Method>::s_null},
            {0},
            0,
            0
        };
        raw<RTTI::Class> result = { &s_class };
        return result;
    }
    static inline raw<const RTTI::Class> klass()
    {
        return preklass();
    }
};

}

/**************************************************************************************************/
#endif
