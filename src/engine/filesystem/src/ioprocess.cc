/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <ioprocess.hh>

namespace BugEngine { namespace IOProcess
{

IOContext::IOContext()
    :   m_availableTickets(0)
    ,   m_freeSlots(128)
    ,   m_firstFreeSlot(i_u32::Zero)
    ,   m_firstUsedSlot(i_u32::Zero)
    ,   m_ioDone(i_u8::Zero)
    ,   m_ioThread("IOThread", &IOContext::ioProcess, reinterpret_cast<intptr_t>(this), 0, Thread::Highest)
{
};

IOContext::~IOContext()
{
    m_ioDone++;
    m_availableTickets.release(1);
    m_ioThread.wait();
}

intptr_t IOContext::ioProcess(intptr_t p1, intptr_t /*p2*/)
{
    IOContext* context = reinterpret_cast<IOContext*>(p1);
    while(1)
    {
        context->m_availableTickets.wait();
        u32 slotIndex = context->m_firstUsedSlot % SlotCount;
        ref<File::Ticket> request = context->m_ticketPool[slotIndex];
        if (!request)
        {
            be_assert(context->m_ioDone, "IO context exited but was not yet finished");
            break;
        }
        context->m_ticketPool[slotIndex] = ref<File::Ticket>();
        context->m_firstUsedSlot++;
        context->m_freeSlots.release(1);

        switch(request->action)
        {
        case File::Ticket::Read:
            if (!context->m_ioDone)
            {
                request->buffer.realloc(request->total);
                request->file->fillBuffer(request);
            }
            break;
        case File::Ticket::Write:
            request->file->writeBuffer(request);
            break;
        default:
            be_error("unknown IO request: %d" | request->action);
            break;
        }
    }
    return 0;
}


void IOContext::pushTicket(ref<File::Ticket> ticket)
{
    m_freeSlots.wait();
    u32 slot = m_firstFreeSlot++;
    be_assert(slot < m_firstUsedSlot + SlotCount, "circular buffer inconsistency");
    slot = slot % SlotCount;
    m_ticketPool[slot] = ticket;
    m_availableTickets.release(1);
}

}}
