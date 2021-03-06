/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/kernel/input/segment.hh>
#include <bugengine/minitl/typemanipulation.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>
#include <bugengine/scheduler/kernel/parameters/parameters.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Segment : public IParameter
{
public:
    Segment()
    {
    }
    ~Segment()
    {
    }
};

template < typename T >
struct ParamTypeToKernelType< ::Kernel::segment< T > >
{
    typedef Segment< typename minitl::remove_const< T >::type > Type;
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/segment.factory.hh>

/**************************************************************************************************/
#endif
