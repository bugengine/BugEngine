/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_STREAM_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_STREAM_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine {

namespace KernelScheduler {
class IParameter;
template < typename T >
class Stream;
}  // namespace KernelScheduler

namespace RTTI {

template < typename T >
struct ClassID< KernelScheduler::Stream< T > >
{
    static BE_EXPORT raw< const RTTI::Class > klass()
    {
        static const RTTI::Class s_class
            = {istring(minitl::format< 256u >("Stream<%s>") | be_type< T >().name()),
               u32(sizeof(KernelScheduler::Stream< T >)),
               0,
               RTTI::ClassType_Object,
               {0},
               {be_class< KernelScheduler::IParameter >().m_ptr},
               {0},
               {0},
               {0, 0},
               {0, 0},
               {0},
               {0},
               0,
               0};
        static RTTI::ObjectInfo registry
            = {be_bugengine_Namespace_BugEngine_KernelScheduler()->objects,
               {0},
               s_class.name,
               RTTI::Value(s_class)};
        static const RTTI::ObjectInfo* ptr
            = be_bugengine_Namespace_BugEngine_KernelScheduler()->objects.set(&registry);
        be_forceuse(ptr);

        raw< const RTTI::Class > result = {&s_class};
        return result;
    }
};

}  // namespace RTTI
}  // namespace BugEngine

/**************************************************************************************************/
#endif
