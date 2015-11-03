/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_RANGE_ONESTEP_INL_
#define BE_SCHEDULER_RANGE_ONESTEP_INL_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/range/onestep.hh>

namespace BugEngine { namespace Task
{

inline range_onestep::range_onestep()
{
}

inline range_onestep::~range_onestep()
{
}

inline size_t range_onestep::size() const
{
    return 1;
}

inline bool range_onestep::atomic() const
{
    return true;
}

inline u32 range_onestep::partCount(u32 /*workerCount*/) const
{
    return 1;
}

inline range_onestep range_onestep::part(u32 index, u32 total) const
{
    be_assert(index == 0, "onestep index can only be 0");
    be_assert(total == 1, "onestep total can only be 1");
    be_forceuse(index);
    be_forceuse(total);
    return *this;
}

}}

/**************************************************************************************************/
#endif
