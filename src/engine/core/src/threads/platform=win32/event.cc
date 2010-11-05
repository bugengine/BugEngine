/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/event.hh>


namespace BugEngine
{

/* condition variables are only supported in Vista+ */
Event::Event()
:   m_data(CreateEvent(0, FALSE, FALSE, 0))
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
    DWORD rcode = WaitForSingleObject((HANDLE)m_data, INFINITE);
    switch(rcode)
    {
    case WAIT_OBJECT_0:
        return Finished;
    case WAIT_FAILED:
        be_notreached();
    case WAIT_ABANDONED:
    default:
        return Abandoned;
    }
}


}
