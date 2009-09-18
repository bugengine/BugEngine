/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>


namespace BugEngine
{

Semaphore::Semaphore(int initialCount, int maxCount)
:   m_data(CreateSemaphore(NULL, initialCount, maxCount, NULL))
{
}

Semaphore::~Semaphore()
{
    CloseHandle((HANDLE)m_data);
}

void Semaphore::release(int count)
{
    ReleaseSemaphore((HANDLE)m_data, count, NULL);
}

Threads::Waitable::WaitResult Semaphore::wait(unsigned int timeout)
{
    DWORD rcode = WaitForSingleObject((HANDLE)m_data, timeout);
    switch(rcode)
    {
    case WAIT_OBJECT_0:
        return Finished;
    case WAIT_TIMEOUT:
        return TimeOut;
    case WAIT_FAILED:
        be_notreached();
    case WAIT_ABANDONED:
    default:
        return Abandoned;
    }
}


}
