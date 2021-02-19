/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IMAGE3D_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IMAGE3D_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine {

namespace KernelScheduler {
class IParameter;
template < typename T >
class Image3D;
}  // namespace KernelScheduler

namespace RTTI {

template < typename T >
struct ClassID< KernelScheduler::Image3D< T > >
{
    static Value construct(Value* parameters, u32 parameterCount)
    {
        be_assert(parameterCount == 0, "too many parameters");
        be_forceuse(parameters);
        return Value(ref< KernelScheduler::Image3D< T > >::create(Arena::task()));
    }
    static const RTTI::Method::Overload s_ctrOverload;
    static const RTTI::Method           s_ctr;
    static RTTI::ObjectInfo             s_productClass;

    static BE_EXPORT raw< const RTTI::Class > klass()
    {
        static const RTTI::Class s_class
            = {istring(minitl::format< 256u >("Image3D<%s>") | be_class< T >()->name),
               u32(sizeof(KernelScheduler::Image1D< T >)),
               0,
               RTTI::ClassType_Object,
               {KernelScheduler::IParameter::getNamespace().m_ptr},
               be_class< KernelScheduler::IParameter >(),
               {&s_productClass},
               {0},
               {0, 0},
               {1, &s_ctr},
               {&s_ctr},
               {0},
               0,
               0};
        raw< const RTTI::Class > result = {&s_class};

        static RTTI::ObjectInfo registry = {KernelScheduler::IParameter::getNamespace()->objects,
                                            {0},
                                            s_class.name,
                                            RTTI::Value(result)};
        static const RTTI::ObjectInfo* ptr
            = KernelScheduler::IParameter::getNamespace()->objects.set(&registry);
        be_forceuse(ptr);

        return result;
    }
};

template < typename T >
const RTTI::Method::Overload ClassID< KernelScheduler::Image3D< T > >::s_ctrOverload
    = {{0}, {0, 0}, be_type< ref< KernelScheduler::Image3D< T > > >(), false, {0, 0}, &construct};

template < typename T >
const RTTI::Method ClassID< KernelScheduler::Image3D< T > >::s_ctr
    = {RTTI::Class::nameConstructor(), {1, &s_ctrOverload}, {&s_ctr}};

template < typename T >
RTTI::ObjectInfo ClassID< KernelScheduler::Image3D< T > >::s_productClass
    = {{0},
       {0},
       KernelScheduler::IParameter::getProductTypePropertyName(),
       Value(be_type< ref< KernelScheduler::Product< KernelScheduler::Image3D< T > > > >())};

}  // namespace RTTI
}  // namespace BugEngine

/**************************************************************************************************/
#endif
