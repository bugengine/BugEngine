/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine {

namespace KernelScheduler {
class IParameter;
template < typename T >
class Segment;
}  // namespace KernelScheduler

namespace RTTI {

template < typename T >
struct ClassID< KernelScheduler::Segment< T > >
{
    static BE_EXPORT raw< const RTTI::Class > klass()
    {
        static const RTTI::Class s_class
           = {istring(minitl::format< 256u >("Segment<%s>") | be_type< T >().name()),
              u32(sizeof(KernelScheduler::Segment< T >)),
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
        raw< const RTTI::Class > result = {&s_class};
        return result;
    }
};

}  // namespace RTTI
}  // namespace BugEngine

/**************************************************************************************************/
#endif
