/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IMAGE3D_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IMAGE3D_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/iparameter.script.hh>


namespace BugEngine { namespace KernelScheduler
{

template< typename T >
class Image3D : public IParameter
{
public:
    Image3D() {}
    ~Image3D() {}
};

}}

#include    <scheduler/kernel/parameters/image3d.factory.hh>

/**************************************************************************************************/
#endif
