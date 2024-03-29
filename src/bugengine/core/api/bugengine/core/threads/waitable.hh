/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_WAITABLE_HH_
#define BE_CORE_THREADS_WAITABLE_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>

namespace BugEngine { namespace Threads {

class be_api(CORE) Waitable
{
public:
    Waitable()
    {
    }
    virtual ~Waitable()
    {
    }

    enum WaitResult
    {
        Finished,
        Abandoned
    };

protected:
    virtual WaitResult wait() = 0;
};

}}  // namespace BugEngine::Threads

/**************************************************************************************************/
#endif
