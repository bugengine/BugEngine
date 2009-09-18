/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_RANGE_ONESTEP_HH_
#define BE_SYSTEM_SCHEDULER_RANGE_ONESTEP_HH_
/*****************************************************************************/

namespace BugEngine
{

class range_onestep
{
public:
    range_onestep();
    ~range_onestep();

    inline size_t               size() const;
    inline range_onestep        split();
    inline bool                 atomic() const;
};

}

#include    "onestep.inl"

/*****************************************************************************/
#endif
