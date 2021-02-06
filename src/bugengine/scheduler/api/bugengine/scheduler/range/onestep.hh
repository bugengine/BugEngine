/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_RANGE_ONESTEP_HH_
#define BE_SCHEDULER_RANGE_ONESTEP_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>

namespace BugEngine { namespace Task {

class range_onestep
{
public:
    range_onestep();
    ~range_onestep();

    inline size_t        size() const;
    inline u32           partCount(u32 workerCount) const;
    inline range_onestep part(u32 index, u32 total) const;
    inline bool          atomic() const;
};

}}  // namespace BugEngine::Task

#include <bugengine/scheduler/range/onestep.inl>

/**************************************************************************************************/
#endif
