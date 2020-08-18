/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETERS_IMAGE1D_HH_
#define BE_SCHEDULER_KERNEL_PARAMETERS_IMAGE1D_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Image1D : public IParameter
{
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
