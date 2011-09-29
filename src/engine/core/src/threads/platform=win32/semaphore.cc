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

Threads::Waitable::WaitResult Semaphore::wait()
{
    DWORD rcode = WaitForSingleObject((HANDLE)m_data, INFINITE);
    switch(rcode)
    {
    case WAIT_OBJECT_0:
        return Finished;
    case WAIT_FAILED:
        be_notreached();
        return Abandoned;
    case WAIT_ABANDONED:
    default:
        return Abandoned;
    }
}


}
