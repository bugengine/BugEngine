/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_TIMER_HH_
#define BE_CORE_TIMER_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>

namespace BugEngine {

class be_api(CORE) Timer
{
private:
    u64 m_total;
    u64 m_start;

private:
    static u64 tick();

public:
    Timer();
    ~Timer();

    void start();
    u64  stop();
    void reset();
    u64  total() const;
    u64  elapsed() const;

    static float now();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
