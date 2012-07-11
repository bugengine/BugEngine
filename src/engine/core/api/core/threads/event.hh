/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_EVENT_HH_
#define BE_CORE_THREADS_EVENT_HH_
/*****************************************************************************/
#include    <core/threads/waitable.hh>

namespace BugEngine
{

class be_api(CORE) Event : public Threads::Waitable
{
private:
    void*   m_data;
    void*   m_lock;
public:
    Event();
    ~Event();

    void set();
    void pulse();
    void lock();
    void unlock();
    virtual Waitable::WaitResult wait() override;
};

}

/*****************************************************************************/
#endif
