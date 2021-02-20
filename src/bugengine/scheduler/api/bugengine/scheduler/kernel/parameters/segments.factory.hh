/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine {

namespace KernelScheduler {
class IParameter;
template < typename T >
class Segments;
}  // namespace KernelScheduler

namespace RTTI {

template < typename T >
struct ClassID< KernelScheduler::Segments< T > >
{
    static Value construct(Value* parameters, u32 parameterCount)
    {
        be_assert(parameterCount == 0, "too many parameters");
        be_forceuse(parameters);
        return Value(ref< KernelScheduler::Segments< T > >::create(Arena::task()));
    }
    static const RTTI::Method::Overload s_ctrOverload;
    static const RTTI::Method           s_ctr;
    static RTTI::ObjectInfo             s_productClass;
    static BE_EXPORT KernelScheduler::IParameter::ParameterRegistration s_registration;

    static BE_EXPORT raw< const RTTI::Class > klass()
    {
        static const RTTI::Class s_class
            = {istring(minitl::format< 256u >("Segments<%s>") | be_class< T >()->name),
               u32(sizeof(KernelScheduler::Segments< T >)),
               0,
               RTTI::ClassType_Object,
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
        raw< const RTTI::Class > result = {&s_class};
        (void)s_registration;
        return result;
    }
};

template < typename T >
const RTTI::Method::Overload ClassID< KernelScheduler::Segments< T > >::s_ctrOverload
    = {{0}, {0, 0}, be_type< ref< KernelScheduler::Segments< T > > >(), false, {0, 0}, &construct};

template < typename T >
const RTTI::Method ClassID< KernelScheduler::Segments< T > >::s_ctr
    = {RTTI::Class::nameConstructor(), {1, &s_ctrOverload}, {&s_ctr}};

template < typename T >
RTTI::ObjectInfo ClassID< KernelScheduler::Segments< T > >::s_productClass
    = {{0},
       {0},
       KernelScheduler::IParameter::getProductTypePropertyName(),
       Value(be_type< ref< KernelScheduler::Product< KernelScheduler::Segments< T > > > >())};

template < typename T >
KernelScheduler::IParameter::ParameterRegistration
    ClassID< KernelScheduler::Segments< T > >::s_registration(klass());

}  // namespace RTTI
}  // namespace BugEngine

/**************************************************************************************************/
#endif
