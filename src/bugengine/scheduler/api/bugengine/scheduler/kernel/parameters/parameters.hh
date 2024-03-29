/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_PARAMETERS_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_PARAMETERS_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
struct ParamTypeToKernelType
{
    // typedef TODO Type;
};

template < typename T >
struct ParamTypeToKernelType< T* >
{
    // typedef TODO Type;
};

template < typename T >
struct ParamTypeToKernelType< T& >
{
    // typedef TODO Type;
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/image1d.hh>
#include <bugengine/scheduler/kernel/parameters/image2d.hh>
#include <bugengine/scheduler/kernel/parameters/image3d.hh>
#include <bugengine/scheduler/kernel/parameters/segment.hh>
#include <bugengine/scheduler/kernel/parameters/segments.hh>
#include <bugengine/scheduler/kernel/parameters/stream.hh>

/**************************************************************************************************/
#endif
