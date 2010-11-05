/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/mutex.hh>
#include    <ogc/mutex.h>


namespace BugEngine
{

Mutex::Mutex()
:   m_data(new mutex_t)
{
    LWP_MutexInit(reinterpret_cast<mutex_t*>(m_data), true);
}

Mutex::~Mutex()
{
    LWP_MutexDestroy(*reinterpret_cast<mutex_t*>(m_data));
    delete reinterpret_cast<mutex_t*>(m_data);
}

void Mutex::release()
{
    LWP_MutexUnlock(*reinterpret_cast<mutex_t*>(m_data));
}

Threads::Waitable::WaitResult Mutex::wait()
{
    LWP_MutexLock(*reinterpret_cast<mutex_t*>(m_data));
    return Finished;
}

}
