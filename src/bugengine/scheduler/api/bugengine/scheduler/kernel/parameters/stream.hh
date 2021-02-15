/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_STREAM_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_STREAM_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

#include <bugengine/scheduler/kernel/product.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Stream : public IParameter
{
protected:
    RTTI::Value makeProduct(ref< IParameter > parameter, weak< Task::ITask > task)
    {
        return RTTI::Value(ref< Product< Stream< T > > >::create(
            Arena::task(), be_checked_cast< Stream< T > >(parameter), task));
    }

public:
    Stream()
    {
    }
    ~Stream()
    {
    }
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/stream.factory.hh>

/**************************************************************************************************/
#endif
