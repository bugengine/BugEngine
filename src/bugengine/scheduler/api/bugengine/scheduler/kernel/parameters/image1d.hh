/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETERS_IMAGE1D_HH_
#define BE_SCHEDULER_KERNEL_PARAMETERS_IMAGE1D_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

#include <bugengine/scheduler/kernel/product.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Image1D : public IParameter
{
protected:
    RTTI::Value makeProduct(ref< IParameter > parameter, weak< Task::ITask > task)
    {
        return RTTI::Value(ref< Product< Image1D< T > > >::create(
            Arena::task(), be_checked_cast< Image1D< T > >(parameter), task));
    }

public:
    Image1D()
    {
    }
    ~Image1D()
    {
    }
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/image1d.factory.hh>

/**************************************************************************************************/
#endif
