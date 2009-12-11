/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/mutex.hh>
#include    <pthread.h>


namespace BugEngine
{

Mutex::Mutex()
:   m_data(new pthread_mutex_t)
{
    pthread_mutex_init(reinterpret_cast<pthread_mutex_t*>(m_data), 0);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(m_data));
    delete reinterpret_cast<pthread_mutex_t*>(m_data);
}

void Mutex::release()
{
    pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(m_data));
}

Threads::Waitable::WaitResult Mutex::wait(unsigned int timeout)
{
    pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(m_data));
    return Finished;
}

}
