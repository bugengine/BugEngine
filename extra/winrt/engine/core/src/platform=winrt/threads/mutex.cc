/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/mutex.hh>


namespace BugEngine
{

Mutex::Mutex()
:   m_data(CreateMutexEx(0, 0, 0, 0))
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

Threads::Waitable::WaitResult Mutex::wait()
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
