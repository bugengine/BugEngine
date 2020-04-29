/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_FACTORY_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>

namespace BugEngine
{

namespace KernelScheduler
{
class IProduct;
template< typename T >
class Product;
}

namespace RTTI
{

template< typename T >
struct ClassID<  KernelScheduler::Product<T> >
{
    static BE_EXPORT raw<const RTTI::Class> klass()
    {
        static const RTTI::Class s_class = {
            istring(minitl::format<256u>("Product<%s>") | be_type<T>().name()),
            u32(sizeof(KernelScheduler::Product<T>)),
            0,
            RTTI::ClassType_Object,
            {0},
            {be_class< KernelScheduler::IProduct >().m_ptr},
            {0},
            { 0 },
            { 0, 0 },
            { 0, 0 },
            { 0 },
            { 0 },
            0,
            0
        };
        raw< const RTTI::Class > result = { &s_class };
        return result;
    }
};

}}

/**************************************************************************************************/
#endif
