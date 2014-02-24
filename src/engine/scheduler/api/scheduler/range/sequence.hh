/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_RANGE_SEQUENCE_HH_
#define BE_SCHEDULER_RANGE_SEQUENCE_HH_
/**************************************************************************************************/

namespace BugEngine { namespace Task
{

template< typename T >
class range_sequence
{
private:
    T       m_begin;
    T       m_end;
    size_t  m_grain;
public:
    range_sequence(T begin, T end, size_t grain = 1);
    ~range_sequence();

    inline T&                   begin();
    inline T&                   end();
    inline size_t               size() const;
    inline range_sequence<T>    split();
    inline bool                 atomic() const;
};

}}

#include    "sequence.inl"

/**************************************************************************************************/
#endif
