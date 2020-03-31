/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/iparameter.script.hh>
#include    <scheduler/kernel/parameters/parameters.hh>
#include    <kernel/input/segment.hh>
#include    <minitl/typemanipulation.hh>


namespace BugEngine { namespace KernelScheduler
{

template< typename T >
class Segment : public IParameter
{
public:
    Segment() {}
    ~Segment() {}
};

template< typename T >
struct ParamTypeToKernelType< ::Kernel::segment<T> >
{
    typedef Segment<typename minitl::remove_const<T>::type> Type;
};

}}

#include    <scheduler/kernel/parameters/segment.factory.hh>

/**************************************************************************************************/
#endif
