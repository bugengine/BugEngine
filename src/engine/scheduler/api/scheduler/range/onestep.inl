/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_RANGE_ONESTEP_INL_
#define BE_SCHEDULER_RANGE_ONESTEP_INL_
/*****************************************************************************/

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

inline range_onestep range_onestep::split()
{
    be_unimplemented();
    return *this;
}

}}

/*****************************************************************************/
#endif
