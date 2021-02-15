/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IMAGE2D_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IMAGE2D_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

#include <bugengine/scheduler/kernel/product.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Image2D : public IParameter
{
protected:
    RTTI::Value makeProduct(ref< IParameter > parameter, weak< Task::ITask > task)
    {
        return RTTI::Value(ref< Product< Image2D< T > > >::create(
            Arena::task(), be_checked_cast< Image2D< T > >(parameter), task));
    }

public:
    Image2D()
    {
    }
    ~Image2D()
    {
    }
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/image2d.factory.hh>

/**************************************************************************************************/
#endif
