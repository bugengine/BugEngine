/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_EVENT_HH_
#define BE_CORE_THREADS_EVENT_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>
#include <bugengine/core/threads/waitable.hh>

namespace BugEngine {

class be_api(CORE) Event : public Threads::Waitable
{
private:
    void* m_data;
    void* m_lock;

public:
    Event();
    ~Event();

    void                         set();
    void                         pulse();
    void                         lock();
    void                         unlock();
    virtual Waitable::WaitResult wait() override;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
