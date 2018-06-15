/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_PARAMETERS_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_PARAMETERS_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>

namespace BugEngine { namespace KernelScheduler
{

template< typename T >
struct ParamTypeToKernelType
{
    //typedef TODO Type;
};

template< typename T >
struct ParamTypeToKernelType<T*>
{
    //typedef TODO Type;
};

template< typename T >
struct ParamTypeToKernelType<T&>
{
    //typedef TODO Type;
};

}}

#include    <scheduler/kernel/parameters/segment.hh>
#include    <scheduler/kernel/parameters/segments.hh>
#include    <scheduler/kernel/parameters/stream.hh>
#include    <scheduler/kernel/parameters/image1d.hh>
#include    <scheduler/kernel/parameters/image2d.hh>
#include    <scheduler/kernel/parameters/image3d.hh>

/**************************************************************************************************/
#endif
