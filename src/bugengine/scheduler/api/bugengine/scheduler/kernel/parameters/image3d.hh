/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IMAGE3D_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IMAGE3D_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

#include <bugengine/scheduler/kernel/product.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Image3D : public IParameter
{
protected:
    RTTI::Value makeProduct(ref< IParameter > parameter, weak< Task::ITask > task)
    {
        return RTTI::Value(ref< Product< Image3D< T > > >::create(
            Arena::task(), be_checked_cast< Image3D< T > >(parameter), task));
    }

public:
    Image3D()
    {
    }
    ~Image3D()
    {
    }
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/image3d.factory.hh>

/**************************************************************************************************/
#endif
