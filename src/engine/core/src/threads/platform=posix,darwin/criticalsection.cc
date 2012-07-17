/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/criticalsection.hh>
#include    <pthread.h>

namespace BugEngine
{

CriticalSection::CriticalSection()
:   m_data(new pthread_mutex_t)
{
    pthread_mutex_init(reinterpret_cast<pthread_mutex_t*>(m_data), 0);
}

CriticalSection::~CriticalSection()
{
    pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(m_data));
    delete reinterpret_cast<pthread_mutex_t*>(m_data);
}

void CriticalSection::enter() const
{
    pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(m_data));
}

void CriticalSection::leave() const
{
    pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(m_data));
}

}
