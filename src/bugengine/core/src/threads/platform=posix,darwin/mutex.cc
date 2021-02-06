/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/threads/mutex.hh>

#include <pthread.h>

namespace BugEngine {

Mutex::Mutex() : m_data(new pthread_mutex_t)
{
    pthread_mutex_init(reinterpret_cast< pthread_mutex_t* >(m_data), 0);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(reinterpret_cast< pthread_mutex_t* >(m_data));
    delete reinterpret_cast< pthread_mutex_t* >(m_data);
}

void Mutex::release()
{
    pthread_mutex_unlock(reinterpret_cast< pthread_mutex_t* >(m_data));
}

Threads::Waitable::WaitResult Mutex::wait()
{
    pthread_mutex_lock(reinterpret_cast< pthread_mutex_t* >(m_data));
    return Finished;
}

}  // namespace BugEngine
