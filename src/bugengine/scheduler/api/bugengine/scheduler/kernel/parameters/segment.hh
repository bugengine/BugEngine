/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/kernel/input/segment.hh>
#include <bugengine/minitl/typemanipulation.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>
#include <bugengine/scheduler/kernel/product.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Segment : public IParameter
{
protected:
    ref< IProduct > makeProduct(ref< IParameter > parameter, weak< Task::ITask > task)
    {
        return ref< Product< Segment< T > > >::create(
            Arena::task(), be_checked_cast< Segment< T > >(parameter), task);
    }

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
