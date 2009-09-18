/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/mutex.hh>


namespace BugEngine
{

Mutex::Mutex()
:   m_data(CreateMutex(0, FALSE, 0))
{
}

Mutex::~Mutex()
{
    CloseHandle((HANDLE)m_data);
}

void Mutex::release()
{
    ReleaseMutex((HANDLE)m_data);
}

Threads::Waitable::WaitResult Mutex::wait(unsigned int timeout)
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
