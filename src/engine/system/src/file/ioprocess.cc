/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <ioprocess.hh>

namespace BugEngine { namespace IOProcess
{

IOContext::IOContext()
    :   m_ioDone(i_u8::Zero)
    ,   m_ioSemaphore(0)
    ,   m_ioThread("IOThread", &IOContext::ioProcess, reinterpret_cast<intptr_t>(this), 0, Thread::Highest)
{
};

IOContext::~IOContext()
{
    m_ioDone++;
    m_ioSemaphore.release(1);
    m_ioThread.wait();
    be_assert(m_tickets.empty(), "Tickets still in queue when exiting IO process");
}

intptr_t IOContext::ioProcess(intptr_t p1, intptr_t /*p2*/)
{
    IOContext* context = reinterpret_cast<IOContext*>(p1);
    while(1)
    {
        context->m_ioSemaphore.wait();
        File::Ticket* request = context->m_requests.pop();
        if (!request)
        {
            be_assert(context->m_ioDone, "IO context exited but was not yet finished");
            break;
        }
        context->m_tickets.push_front(*request);
        File::Ticket* t = context->m_tickets.begin().operator->();
        context->m_tickets.erase(context->m_tickets.begin());
        switch(t->action)
        {
        case File::Ticket::Read:
            if (!context->m_ioDone)
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


void IOContext::pushTicket(ref<File::Ticket> ticket)
{
    File::Ticket* t = ticket.operator->();
    t->addref();
    m_requests.push(t);
    m_ioSemaphore.release(1);
}

}}
