/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETERS_IMAGE1D_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PARAMETERS_IMAGE1D_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/typeinfo.hh>

namespace BugEngine {

namespace KernelScheduler {
class IParameter;
template < typename T >
class Image1D;
}  // namespace KernelScheduler

namespace Meta {

template < typename T >
struct ClassID< KernelScheduler::Image1D< T > >
{
    static Value construct(Value* parameters, u32 parameterCount)
    {
        be_assert(parameterCount == 0, "too many parameters");
        be_forceuse(parameters);
        return Value(ref< KernelScheduler::Image1D< T > >::create(Arena::task()));
    }
    static const Meta::Method::Overload s_ctrOverload;
    static const Meta::Method           s_ctr;
    static Meta::ObjectInfo             s_productClass;
    static BE_EXPORT KernelScheduler::IParameter::ParameterRegistration s_registration;

    static BE_EXPORT raw< const Meta::Class > klass()
    {
        static const Meta::Class s_class
            = {istring(minitl::format< 256u >("Image1D<%s>") | be_class< T >()->name),
               u32(sizeof(KernelScheduler::Image1D< T >)),
               0,
               Meta::ClassType_Object,
               {0},
               be_class< KernelScheduler::IParameter >(),
               {&s_productClass},
               {0},
               {0, 0},
               {1, &s_ctr},
               {&s_ctr},
               {0},
               0,
               0};
        raw< const Meta::Class > result = {&s_class};
        (void)s_registration;
        return result;
    }
};

template < typename T >
const Meta::Method::Overload ClassID< KernelScheduler::Image1D< T > >::s_ctrOverload
    = {{0}, {0, 0}, be_type< ref< KernelScheduler::Image1D< T > > >(), false, {0, 0}, &construct};

template < typename T >
const Meta::Method ClassID< KernelScheduler::Image1D< T > >::s_ctr
    = {Meta::Class::nameConstructor(),
       {1, &s_ctrOverload},
       {&ClassID< KernelScheduler::Image1D< T > >::s_ctr}};

template < typename T >
Meta::ObjectInfo ClassID< KernelScheduler::Image1D< T > >::s_productClass
    = {{0},
       {0},
       KernelScheduler::IParameter::getProductTypePropertyName(),
       Value(be_type< ref< KernelScheduler::Product< KernelScheduler::Image1D< T > > > >())};

template < typename T >
KernelScheduler::IParameter::ParameterRegistration
    ClassID< KernelScheduler::Image1D< T > >::s_registration(klass());

}  // namespace Meta
}  // namespace BugEngine

/**************************************************************************************************/
#endif
