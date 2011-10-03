/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <ioprocess.hh>

#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>

namespace BugEngine
{


static bool         s_ioDone;
static Semaphore    s_ioSemaphore(0, 32768);
static Thread       s_ioThread("IOThread", &IOContext::ioProcess, 0, 0, Thread::Highest);
static IOContext    s_iocontext;

IOContext::IOContext()
{
};

IOContext::~IOContext()
{
    s_ioDone = true;
    s_ioSemaphore.release(1);
}

intptr_t IOContext::ioProcess(intptr_t p1, intptr_t p2)
{
    while(s_ioSemaphore.wait())
    {
        if (s_ioDone)
            break;
        /* doWork*/
    }
    return 0;
}

void IOContext::pushTicket(ref<const File::Ticket> ticket)
{
    s_ioSemaphore.release(1);
}

}
