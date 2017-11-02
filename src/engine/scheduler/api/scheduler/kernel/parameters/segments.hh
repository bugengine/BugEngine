/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace Kernel
{

template< typename T >
class Segments : public IParameter
{
public:
    Segments() {}
    ~Segments() {}
};

}}

/**************************************************************************************************/
#endif
