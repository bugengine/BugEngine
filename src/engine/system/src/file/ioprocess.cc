/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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


static i_u8         s_ioDone(i_u8::Zero);
static Semaphore    s_ioSemaphore(0);
static Thread       s_ioThread("IOThread", &IOContext::ioProcess, 0, 0, Thread::Highest);
static IOContext    s_iocontext;

IOContext::IOContext()
{
};

IOContext::~IOContext()
{
    s_ioDone++;
    s_ioSemaphore.release(1);
    s_ioThread.wait();
    be_assert(s_iocontext.tickets.empty(), "Tickets still in queue when exiting IO process");
}

intptr_t IOContext::ioProcess(intptr_t /*p1*/, intptr_t /*p2*/)
{
    while(1)
    {
        s_ioSemaphore.wait();
        File::Ticket* request = s_iocontext.requests.pop();
        if (!request)
        {
            be_assert(s_ioDone, "IO context exited but was not yet finished");
            break;
        }
        s_iocontext.tickets.push_front(*request);
        File::Ticket* t = s_iocontext.tickets.begin().operator->();
        s_iocontext.tickets.erase(s_iocontext.tickets.begin());
        switch(t->action)
        {
        case File::Ticket::Read:
            if (!s_ioDone)
            {
                t->buffer.realloc(t->total);
                t->file->fillBuffer(t);
            }
            break;
        case File::Ticket::Write:
            t->file->writeBuffer(t);
            break;
        default:
            be_error("unknown IO request: %d" | t->action);
            break;
        }
        t->decref();
    }
    return 0;
}


void pushTicket(ref<File::Ticket> ticket)
{
    File::Ticket* t = ticket.operator->();
    t->addref();
    s_iocontext.requests.push(t);
    s_ioSemaphore.release(1);
}

}}
