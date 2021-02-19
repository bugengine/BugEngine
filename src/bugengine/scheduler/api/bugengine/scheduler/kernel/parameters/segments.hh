/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/kernel/input/segments.hh>
#include <bugengine/minitl/typemanipulation.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>
#include <bugengine/scheduler/kernel/product.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Segments : public IParameter
{
protected:
    ref< IProduct > makeProduct(ref< IParameter > parameter, weak< Task::ITask > task)
    {
        return ref< Product< Segments< T > > >::create(
            Arena::task(), be_checked_cast< Segments< T > >(parameter), task);
    }

public:
    Segments()
    {
    }
    ~Segments()
    {
    }
};

template < typename T >
struct ParamTypeToKernelType< ::Kernel::segments< T > >
{
    typedef Segments< typename minitl::remove_const< T >::type > Type;
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/segments.factory.hh>

/**************************************************************************************************/
#endif
