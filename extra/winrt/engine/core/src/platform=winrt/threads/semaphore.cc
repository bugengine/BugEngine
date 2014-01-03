/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <windows.h>

namespace BugEngine
{

Semaphore::Semaphore(int initialCount)
    :   m_data(CreateSemaphoreExW(NULL, initialCount, 65535, NULL, 0, 0))
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
    DWORD rcode = WaitForSingleObjectEx((HANDLE)m_data, INFINITE, FALSE);
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
