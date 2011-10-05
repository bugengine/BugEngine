/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <ioprocess.hh>

#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>

namespace BugEngine { namespace IOProcess
{

struct IOContext
{
    IOContext();
    ~IOContext();

    minitl::intrusive_list<File::Ticket> tickets;
    minitl::istack<File::Ticket> requests;
    static intptr_t ioProcess(intptr_t p1, intptr_t p2);
};


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
    s_ioThread.wait();
    for( minitl::intrusive_list<File::Ticket>::iterator it = s_iocontext.tickets.begin(); it != s_iocontext.tickets.end(); )
    {
        File::Ticket* t = it.operator->();
        it = s_iocontext.tickets.erase(it);
        minitl::decref(t);
    }
    while(File::Ticket* t = requests.pop())
    {
        minitl::decref(t);
    }
    be_assert(s_iocontext.tickets.empty(), "Tickets still in queue when exiting IO process");
}

intptr_t IOContext::ioProcess(intptr_t p1, intptr_t p2)
{
    while(1)
    {
        s_ioSemaphore.wait();
        if (s_ioDone)
            break;
        File::Ticket* request = s_iocontext.requests.pop();
        s_iocontext.tickets.push_back(*request);
        //s_iocontext.sort();
        File::Ticket* t = s_iocontext.tickets.begin().operator->();
        s_iocontext.tickets.erase(s_iocontext.tickets.begin());
        t->file->fillBuffer(t);
        minitl::decref(t);
    }
    return 0;
}


void pushTicket(ref<File::Ticket> ticket)
{
    File::Ticket* t = ticket.operator->();
    minitl::addref(t);
    s_iocontext.requests.push(t);
    s_ioSemaphore.release(1);
}

}}
