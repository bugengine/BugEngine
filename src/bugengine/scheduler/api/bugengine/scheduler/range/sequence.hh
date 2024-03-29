/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_RANGE_SEQUENCE_HH_
#define BE_SCHEDULER_RANGE_SEQUENCE_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>

namespace BugEngine { namespace Task {

template < typename T >
class range_sequence
{
private:
    T      m_begin;
    T      m_end;
    size_t m_grain;

public:
    range_sequence(T begin, T end, size_t grain = 1);
    ~range_sequence();

    inline T&                  begin();
    inline T&                  end();
    inline size_t              size() const;
    inline u32                 partCount(u32 workerCount) const;
    inline range_sequence< T > part(u32 index, u32 total) const;
    inline bool                atomic() const;
};

}}  // namespace BugEngine::Task

#include <bugengine/scheduler/range/sequence.inl>

/**************************************************************************************************/
#endif
