/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/event.hh>


namespace BugEngine
{

/* condition variables are only supported in Vista+ */
Event::Event()
:   m_data(CreateEventEx(0, 0, 0, 0))
{
}

Event::~Event()
{
    CloseHandle((HANDLE)m_data);
}

void Event::set()
{
    SetEvent((HANDLE)m_data);
}

void Event::lock()
{
}

void Event::unlock()
{
}

Threads::Waitable::WaitResult Event::wait()
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
