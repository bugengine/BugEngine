/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENTS_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/iparameter.script.hh>
#include    <scheduler/kernel/parameters/parameters.hh>
#include    <kernel/input/segments.hh>
#include    <minitl/typemanipulation.hh>


namespace BugEngine { namespace KernelScheduler
{

template< typename T >
class Segments : public IParameter
{
public:
    Segments() {}
    ~Segments() {}
};

template< typename T >
struct ParamTypeToKernelType< ::Kernel::segments<T> >
{
    typedef Segments<typename minitl::remove_const<T>::type> Type;
};

}}

#include    <scheduler/kernel/parameters/segments.factory.hh>

/**************************************************************************************************/
#endif
