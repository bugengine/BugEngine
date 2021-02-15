/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PRODUCT_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PRODUCT_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/iproduct.script.hh>

#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine {

namespace KernelScheduler {

template < typename T >
class Product;
}  // namespace KernelScheduler

namespace RTTI {

template < typename T >
struct ClassID< KernelScheduler::Product< T > >
{
    static BE_EXPORT raw< const RTTI::Class > klass()
    {
        static const RTTI::Class s_class
            = {istring(minitl::format< 256u >("Product<%s>") | be_class< T >()->name),
               u32(sizeof(KernelScheduler::Product< T >)),
               0,
               RTTI::ClassType_Object,
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
        raw< const RTTI::Class > result = {&s_class};

        static RTTI::ObjectInfo registry = {KernelScheduler::IProduct::getNamespace()->objects,
                                            {0},
                                            s_class.name,
                                            RTTI::Value(result)};
        static const RTTI::ObjectInfo* ptr
            = KernelScheduler::IProduct::getNamespace()->objects.set(&registry);
        be_forceuse(ptr);

        return result;
    }
};

}  // namespace RTTI
}  // namespace BugEngine

/**************************************************************************************************/
#endif
