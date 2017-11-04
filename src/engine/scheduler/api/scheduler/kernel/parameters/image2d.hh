/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IMAGE2D_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IMAGE2D_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/iparameter.script.hh>


namespace BugEngine { namespace Kernel
{

template< typename T >
class Image2D : public IParameter
{
public:
    Image2D() {}
    ~Image2D() {}
};

}}

#include    <scheduler/kernel/parameters/image2d.factory.hh>

/**************************************************************************************************/
#endif
