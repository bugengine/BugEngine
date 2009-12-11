/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/criticalsection.hh>
#include    <ogc/mutex.h>

namespace BugEngine
{

CriticalSection::CriticalSection()
:   m_data(new mutex_t)
{
    LWP_MutexInit(reinterpret_cast<mutex_t*>(m_data), true);
}

CriticalSection::~CriticalSection()
{
    LWP_MutexDestroy(*reinterpret_cast<mutex_t*>(m_data));
    delete reinterpret_cast<mutex_t*>(m_data);
}

void CriticalSection::enter()
{
    LWP_MutexLock(*reinterpret_cast<mutex_t*>(m_data));
}

void CriticalSection::leave()
{
    LWP_MutexUnlock(*reinterpret_cast<mutex_t*>(m_data));
}

}
