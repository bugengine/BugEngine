/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_MUTEX_HH_
#define BE_CORE_THREADS_MUTEX_HH_
/*****************************************************************************/
#include    <core/threads/waitable.hh>

namespace BugEngine
{

class COREEXPORT Mutex : public Threads::Waitable
{
private:
    void*           m_data;
public:
    Mutex();
    ~Mutex();

    void release();
    virtual Waitable::WaitResult wait(unsigned int waitTime = Forever) override;
};

class ScopedMutexLock
{
private:
    Mutex&  m_mutex;
public:
    inline ScopedMutexLock(Mutex& m) : m_mutex(m)   { m_mutex.wait(); }
    inline ~ScopedMutexLock()                       { m_mutex.release(); }
private:
    ScopedMutexLock& operator=(const ScopedMutexLock& other);
    ScopedMutexLock(const ScopedMutexLock& other);
};

}

/*****************************************************************************/
#endif
