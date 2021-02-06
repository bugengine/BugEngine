/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_SEMAPHORE_HH_
#define BE_CORE_THREADS_SEMAPHORE_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>
#include <bugengine/core/threads/waitable.hh>

namespace BugEngine {

class be_api(CORE) Semaphore : public Threads::Waitable
{
private:
    void* m_data;

public:
    Semaphore(int initialCount);
    ~Semaphore();

    void                         release(int count);
    virtual Waitable::WaitResult wait() override;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
