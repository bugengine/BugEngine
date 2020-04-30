/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_STREAM_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_STREAM_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace KernelScheduler {

template < typename T >
class Stream : public IParameter
{
public:
    Stream()
    {
    }
    ~Stream()
    {
    }
};

}}  // namespace BugEngine::KernelScheduler

#include <bugengine/scheduler/kernel/parameters/stream.factory.hh>

/**************************************************************************************************/
#endif
