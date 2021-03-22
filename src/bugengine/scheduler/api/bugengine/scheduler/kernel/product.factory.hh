/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/iproduct.script.hh>

#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/typeinfo.hh>

namespace BugEngine {

namespace KernelScheduler {

template < typename T >
class Product;
}  // namespace KernelScheduler

namespace Meta {

template < typename T >
struct ClassID< KernelScheduler::Product< T > >
{
    static BE_EXPORT raw< const Meta::Class > klass()
    {
        static const Meta::Class s_class
            = {istring(minitl::format< 256u >("Product<%s>") | be_class< T >()->name),
               u32(sizeof(KernelScheduler::Product< T >)),
               0,
               Meta::ClassType_Object,
               {KernelScheduler::IProduct::getNamespace().m_ptr},
               {be_class< KernelScheduler::IProduct >().m_ptr},
               {0},
               {0},
               {0, 0},
               {0, 0},
               {0},
               {0},
               0,
               0};
        raw< const Meta::Class > result = {&s_class};

        static Meta::ObjectInfo registry = {KernelScheduler::IProduct::getNamespace()->objects,
                                            {0},
                                            s_class.name,
                                            Meta::Value(result)};
        static const Meta::ObjectInfo* ptr
            = KernelScheduler::IProduct::getNamespace()->objects.set(&registry);
        be_forceuse(ptr);

        return result;
    }
};

}  // namespace Meta
}  // namespace BugEngine

/**************************************************************************************************/
#endif
