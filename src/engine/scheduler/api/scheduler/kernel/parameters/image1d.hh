/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IMAGE1D_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IMAGE1D_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace Kernel
{

template< typename T >
class Image1D : public IParameter
{
public:
    Image1D() {}
    ~Image1D() {}
};

}}

/**************************************************************************************************/
#endif
