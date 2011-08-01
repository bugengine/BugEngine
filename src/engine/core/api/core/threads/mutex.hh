/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_MUTEX_HH_
#define BE_CORE_THREADS_MUTEX_HH_
/*****************************************************************************/
#include    <core/threads/waitable.hh>

namespace BugEngine
{

class ScopedMutexLock;

class be_api(CORE) Mutex : public Threads::Waitable
{
    friend class ScopedMutexLock;
private:
    void*           m_data;
public:
    Mutex();
    ~Mutex();
private:
    void release();
    virtual Waitable::WaitResult wait() override;
};

class ScopedMutexLock
{
    BE_NOCOPY(ScopedMutexLock);
private:
    Mutex&  m_mutex;
public:
    inline ScopedMutexLock(Mutex& m) : m_mutex(m)   { m_mutex.wait(); }
    inline ~ScopedMutexLock()                       { m_mutex.release(); }
};

}

/*****************************************************************************/
#endif
